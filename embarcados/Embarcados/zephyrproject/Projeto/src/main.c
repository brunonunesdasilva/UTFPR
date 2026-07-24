#include "config.h"
// Definição das threads
K_THREAD_STACK_DEFINE(adc_thread_stack, 1024);
K_THREAD_STACK_DEFINE(servo_thread_stack, 1024);
static struct k_thread adc_thread_data;
static struct k_thread servo_thread_data;
static k_tid_t adc_tid;
static k_tid_t servo_tid;

// Prioridades
#define ADC_PRIORITY 5
#define SERVO_PRIORITY 4
#define PWM_CHANNEL 0

// Semáforos e mutexes
K_SEM_DEFINE(servo_update_sem, 0, 1);
K_MUTEX_DEFINE(adc_data_mutex);

// Timer para ADC
K_TIMER_DEFINE(adc_timer, NULL, NULL);

// Verifica ADC
#if !DT_NODE_HAS_STATUS(DT_NODELABEL(adc1), okay)
#error "ADC devicetree node is disabled"
#endif

// Verifica PWM
// #if !DT_NODE_EXISTS(PWM_SERVO_CTLR)
// #error "PWM servo alias not found in devicetree"
// #endif

#define PWM_SERVO_CHANNEL 1

static int16_t adc_sample_buffer[1];

// Configuração do ADC
static const struct adc_channel_cfg channel_cfg = {
    .gain = ADC_GAIN,
    .reference = ADC_REFERENCE,
    .acquisition_time = ADC_ACQUISITION_TIME,
    .channel_id = ADC_CHANNEL_ID,
    .differential = 0,
#ifdef CONFIG_ADC_CONFIGURABLE_INPUTS
    .input_positive = ADC_CHANNEL_ID,
#endif
};

// Configuração da leitura do ADC
static const struct adc_sequence adc_seq = {
    .channels = BIT(ADC_CHANNEL_ID),
    .buffer = adc_sample_buffer,
    .buffer_size = sizeof(adc_sample_buffer),
    .resolution = ADC_RESOLUTION,
};

// Variáveis globais para o servo
static uint32_t current_pulse_us = SERVO_MIN_PULSE_US;
static uint16_t current_angle = 0;
static uint16_t current_adc_value = 0;

// Device do PWM
const struct device *pwm_dev;

// Função para converter valor ADC em pulso PWM
uint32_t adc_to_pulse(uint16_t adc_value)
{
    uint32_t pulse = SERVO_MIN_PULSE_US +
                     ((uint32_t)adc_value * (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US)) / ADC_MAX_VALUE;
    return pulse;
}

// Função para converter pulso em ângulo
uint16_t pulse_to_angle(uint32_t pulse_us)
{
    return ((pulse_us - SERVO_MIN_PULSE_US) * 180) / (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US);
}

// Callback do timer ADC
void adc_timer_callback(struct k_timer *timer)
{
    k_sem_give(&servo_update_sem);
}

// Thread para leitura do ADC
static void adc_thread(void *a, void *b, void *c)
{
    const struct device *adc_dev;
    int32_t ret;
    int32_t adc_raw;

    adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc1));
    if (!device_is_ready(adc_dev))
    {
        printk("ERROR: ADC device not ready\n");
        return;
    }

    ret = adc_channel_setup(adc_dev, &channel_cfg);
    if (ret < 0)
    {
        printk("ERROR: ADC channel setup failed: %d\n", ret);
        return;
    }

    printk("ADC initialized successfully\n");

    k_timer_init(&adc_timer, adc_timer_callback, NULL);
    k_timer_start(&adc_timer, K_MSEC(ADC_READ_INTERVAL_MS), K_MSEC(ADC_READ_INTERVAL_MS));

    while (1)
    {
        k_sem_take(&servo_update_sem, K_FOREVER);

        ret = adc_read(adc_dev, &adc_seq);
        if (ret < 0)
        {
            printk("ERROR: ADC read failed: %d\n", ret);
            continue;
        }

        adc_raw = adc_sample_buffer[0];

        if (k_mutex_lock(&adc_data_mutex, K_MSEC(10)) == 0)
        {
            current_adc_value = (uint16_t)adc_raw;
            current_pulse_us = adc_to_pulse(current_adc_value);
            current_angle = pulse_to_angle(current_pulse_us);
            k_mutex_unlock(&adc_data_mutex);
        }
    }
}

// Thread para controle do servo
static void servo_thread(void *a, void *b, void *c)
{
    int ret;
    uint32_t pulse_us;
    uint16_t angle;
    uint16_t adc_value; // Para leitura segura do ADC
    const struct device *pwm_dev = DEVICE_DT_GET(DT_NODELABEL(pwm1));

    // Declaração do device PWM (local ou global)
    // const struct device *pwm_dev = DEVICE_DT_GET(PWM_SERVO_CTLR);

    if (!device_is_ready(pwm_dev))
    {
        printk("ERROR: PWM device not ready\n");
        return;
    }
    printk("PWM initialized successfully\n");

    // Obtém o device do PWM pelo parent do pwm1
    pwm_dev = DEVICE_DT_GET(DT_PARENT(DT_NODELABEL(pwm1)));
    if (!device_is_ready(pwm_dev))
    {
        printk("ERROR: PWM device not ready\n");
        return;
    }
    printk("PWM initialized successfully\n");

    // Posição inicial (90°)
    uint32_t initial_pulse = (SERVO_MIN_PULSE_US + SERVO_MAX_PULSE_US) / 2;
    ret = pwm_set(pwm_dev, PWM_CHANNEL,
                  PWM_USEC(SERVO_PERIOD_US),
                  PWM_USEC(initial_pulse),
                  PWM_POLARITY_NORMAL);
    if (ret < 0)
    {
        printk("ERROR: Failed to set initial PWM: %d\n", ret);
        return;
    }
    printk("Servo initialized at 90 degrees\n");

    k_msleep(1000);

    uint8_t print_counter = 0;

    while (1)
    {
        // Lê todos os dados de uma vez com o mutex
        if (k_mutex_lock(&adc_data_mutex, K_MSEC(10)) == 0)
        {
            pulse_us = current_pulse_us;
            angle = current_angle;
            adc_value = current_adc_value; // Lê dentro do mutex
            k_mutex_unlock(&adc_data_mutex);
        }
        else
        {
            printk("WARNING: Failed to acquire mutex\n");
            k_msleep(SERVO_UPDATE_INTERVAL_MS);
            continue;
        }

        // Validação adicional (opcional mais recomendado)
        if (pulse_us < SERVO_MIN_PULSE_US || pulse_us > SERVO_MAX_PULSE_US)
        {
            printk("ERROR: Invalid pulse width: %d us\n", pulse_us);
            k_msleep(SERVO_UPDATE_INTERVAL_MS);
            continue;
        }

        // Atualiza PWM do servo
        ret = pwm_set(pwm_dev, PWM_CHANNEL,
                      PWM_USEC(SERVO_PERIOD_US),
                      PWM_USEC(pulse_us),
                      PWM_POLARITY_NORMAL);
        if (ret < 0)
        {
            printk("ERROR: PWM set failed: %d\n", ret);
        }

        // Print periódico (a cada 20 iterações)
        if (++print_counter >= 20)
        {
            printk("ADC: %4d | Pulse: %4d us | Angle: %3d°\n",
                   adc_value, pulse_us, angle);
            print_counter = 0;
        }

        k_msleep(SERVO_UPDATE_INTERVAL_MS);
    }
}

// Comandos via UART
static void uart_cb(const struct device *dev, void *user_data)
{
    static char rx_buf[16];
    static int rx_buf_pos = 0;
    uint8_t c;
    const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(usart2));

    if (!uart_irq_update(uart_dev))
    {
        return;
    }

    if (!uart_irq_rx_ready(uart_dev))
    {
        return;
    }

    while (uart_fifo_read(uart_dev, &c, 1) == 1)
    {
        if ((c == '\r' || c == '\n') && rx_buf_pos > 0)
        {
            rx_buf[rx_buf_pos] = '\0';

            if (strcmp(rx_buf, "status") == 0)
            {
                printk("\n=== SYSTEM STATUS ===\n");
                printk("ADC Value: %d / %d\n", current_adc_value, ADC_MAX_VALUE);
                printk("Pulse Width: %d us\n", current_pulse_us);
                printk("Servo Angle: %d degrees\n", current_angle);
                printk("Uptime: %lld ms\n", k_uptime_get());
                printk("====================\n\n");
            }
            else if (strcmp(rx_buf, "help") == 0)
            {
                printk("\n=== COMMANDS ===\n");
                printk("status - Show system status\n");
                printk("help   - Show this menu\n");
                printk("================\n\n");
            }

            rx_buf_pos = 0;
        }
        else if (rx_buf_pos < (sizeof(rx_buf) - 1))
        {
            rx_buf[rx_buf_pos++] = c;
        }
    }
}

int main(void)
{
    const struct device *uart_dev;

    printk("\n=== STM32 Nucleo-G474RE - Servo Control ===\n");
    printk("Rotate potentiometer to control servo position\n");
    printk("Commands: status, help\n\n");

    sys_heap_init(&app_heap, app_heap_mem, APP_HEAP_SIZE);

    uart_dev = DEVICE_DT_GET(DT_NODELABEL(usart2));
    if (!device_is_ready(uart_dev))
    {
        printk("ERROR: UART device not ready\n");
        return -1;
    }

    uart_irq_callback_user_data_set(uart_dev, uart_cb, NULL);
    uart_irq_rx_enable(uart_dev);

    printk("UART initialized\n");

    adc_tid = k_thread_create(&adc_thread_data, adc_thread_stack,
                              K_THREAD_STACK_SIZEOF(adc_thread_stack),
                              adc_thread, NULL, NULL, NULL, ADC_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(adc_tid, "adc_thread");

    servo_tid = k_thread_create(&servo_thread_data, servo_thread_stack,
                                K_THREAD_STACK_SIZEOF(servo_thread_stack),
                                servo_thread, NULL, NULL, NULL, SERVO_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(servo_tid, "servo_thread");

    printk("All threads started\n");
    printk("System ready!\n\n");

    while (1)
    {
        k_msleep(1000);
    }

    return 0;
}