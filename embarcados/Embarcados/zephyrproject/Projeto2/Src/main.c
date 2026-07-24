// Projeto2 - Controle de servomotor com potenciômetro e botão
// Placa utilizada: Nucleo-g474re

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/sys/util.h>
#include <inttypes.h>
#include <stdlib.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/* 1000 msec = 1 sec  -> delay das piscadas do led*/
#define SLEEP_TIME_MS 1000

// node do led0 devicetree
#define LED0_NODE DT_ALIAS(led0)

// PWM pulso com passo em microssegundos
#define STEP PWM_USEC(100)

// ADC
#define ADC_RESOLUTION 12
#define ADC_CHANNEL 1
#define ADC_NODE DT_NODELABEL(adc1)

// BUTTON
#define SW0_NODE DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

// SERVO
#define SERVO_NODE DT_NODELABEL(servo)

// STATIC GPIO
// button
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button_cb_data;

// led
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// pwm servo
static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_NODELABEL(servo));

// adc
static const struct device *adc_dev;
static int16_t sample_buffer;

static int16_t last_adc_value = -1; // último valor lido do adc

// Controle Thread servo
static bool servo_enabled = false;
K_SEM_DEFINE(servo_sem, 0, 1); // começa bloqueado

// fução para o botão para ligar e desligar o servo
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    servo_enabled = !servo_enabled; // alterna estado

    // se servo está ligado
    if (servo_enabled)
    {
        LOG_INF(">> Servo LIGADO");
    }
    else
    {
        LOG_INF(">> Servo DESLIGADO");
    }
}

// inicialização do ADC
int init_adc(void)
{
    adc_dev = DEVICE_DT_GET(ADC_NODE); // ponteiro para o driver do adc
    if (!device_is_ready(adc_dev))
    {
        LOG_ERR("Dispositivo ADC %s não está pronto", adc_dev->name);
        return -1;
    }

    // configuração de canal, ganho e referência interna
    struct adc_channel_cfg channel_cfg = {
        .channel_id = ADC_CHANNEL,
        .gain = ADC_GAIN_1,
        .reference = ADC_REF_INTERNAL,
        .acquisition_time = ADC_ACQ_TIME_DEFAULT,
    };

    int ret = adc_channel_setup(adc_dev, &channel_cfg); // configura o canal do adc
    if (ret < 0)
    {
        LOG_ERR("Falha ao configurar o canal ADC %d (erro %d)\n", ADC_CHANNEL, ret);
        return ret;
    }

    return 0;
}

// leitor do adc
int16_t read_adc(void)
{
    struct adc_sequence sequence = {
        .channels = BIT(ADC_CHANNEL),
        .buffer = &sample_buffer,
        .buffer_size = sizeof(sample_buffer),
        .resolution = ADC_RESOLUTION,
    };

    int ret = adc_read(adc_dev, &sequence); // lê o valor do potenciometro
    if (ret < 0)
    {
        LOG_ERR("Leitura do ADC falhou (erro %d)\n", ret);
        return ret;
    }

    return sample_buffer;
}

/* protótipo */
void adc_work_handler(struct k_work *work);

// WORKQUEUE para ler ADC periodicamente sem polling na thread
K_WORK_DELAYABLE_DEFINE(adc_work, adc_work_handler);

/* Work Handler — roda a cada 20 ms */
void adc_work_handler(struct k_work *work)
{
    // Sempre reagenda ao final
    k_work_reschedule(&adc_work, K_MSEC(20));

    // le o adc val mesmo com o servo desligado
    int16_t adc_val = read_adc();
    if (servo_enabled)
    {
        /* Filtro anti-ruído */
        if (abs(adc_val - last_adc_value) > 30)
        {
            last_adc_value = adc_val;
            k_sem_give(&servo_sem);
        }
    }
    else
    {
        // atualiza o valor do adc com o servo desligado
        last_adc_value = adc_val;
    }
}

// thread do piscar led
void led_thread(void)
{
    int ret;
    // bool led_state = true;
    LOG_INF("Thread do LED iniciou");

    if (!gpio_is_ready_dt(&led))
    {
        return;
    }

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE); // configura o LED como saída digital e coloca ele em nível lógico alto
    if (ret < 0)
    {
        return;
    }

    while (1)
    {
        ret = gpio_pin_toggle_dt(&led); // alterna o estado lógico do led
        if (ret < 0)
        {
            return;
        }

        // led_state = !led_state; // inverte o valor da variável atual do estado do led
        //  printf("Estado do LED: %s\n", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS); // thread dorme
    }
}

// thread do botão pra ligar o servo
void button_thread(void)
{
    int ret;

    if (!device_is_ready(button.port))
    {
        LOG_ERR("Erro botão %s não está pronto", button.port->name);
        return;
    }

    ret = gpio_pin_configure_dt(&button, GPIO_INPUT); // configura o botão como entrada digital
    if (ret != 0)
    {
        LOG_ERR("Erro %d: falha ao configurar o pino do botão", ret);
        return;
    }

    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE); // configura interrupção do botão
    if (ret != 0)
    {
        LOG_ERR("Erro %d: falha ao configurar a interrupção do botão", ret);
        return;
    }

    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin)); // inicializa o callback
    gpio_add_callback(button.port, &button_cb_data);                      // registra o callback
}

// thread do servo
void servomotor_thread(void)
{
    init_adc();

    LOG_INF("Thread de controle do servo por potenciômetro iniciada");

    while (1)
    {
        k_sem_take(&servo_sem, K_FOREVER); // thread do servo fique parada esperando o botão liberar um semáforo
        if (!servo_enabled)
        {
            continue; // se o servo foi ligado, continua
        }
        int16_t adc_value = last_adc_value; // lê o valor do adc atualizado

        // ADC (0–4095) -> pulso (min_pulse–max_pulse)
        uint32_t pulse_width_us = 1000 + (adc_value * (2000 - 1000) / 4095); // converte adc em largura de pulso (1000 - 2000)

        uint32_t pulse_width = PWM_USEC(pulse_width_us); // converte pra microsegundos

        int ret = pwm_set_dt(&servo, servo.period, pulse_width); // define o pwm
        if (ret < 0)
        {
            LOG_ERR("PWM error: %d\n", ret);
        }

        int graus = (int)(((float)(pulse_width_us - 1000) / (2000 - 1000)) * 120.0f); // transforma a largura de pulso em graus

        // nao deixa o servo ser menor do q 0 graus e nem passar de 120 graus
        // graus = CLAMP(graus, 0, 120);

        LOG_INF("Pulso=%u us | Grau=%d°\n", pulse_width_us, graus);
    }
}

// Configuração das threads com prioridades
K_THREAD_DEFINE(led_thread_id, 1024, led_thread, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(servo_thread_id, 1024, servomotor_thread, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(button_thread_id, 1024, button_thread, NULL, NULL, NULL, 7, 0, 0);

int main(void)
{
    LOG_INF("Hello World! %s", CONFIG_BOARD_TARGET);
    k_work_schedule(&adc_work, K_MSEC(20));

    return 0;
}
