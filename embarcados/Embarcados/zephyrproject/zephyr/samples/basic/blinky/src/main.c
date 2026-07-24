#include "config.h"

// Definição das threads e suas pilhas:
K_THREAD_STACK_DEFINE(blink_thread_stack, 512);
K_THREAD_STACK_DEFINE(uart_thread_stack, 512);
K_THREAD_STACK_DEFINE(adc_thread_stack, 512);
static struct k_thread blink_thread_data;
static struct k_thread uart_thread_data;
static struct k_thread adc_thread_data;
static k_tid_t blink_tid;
static k_tid_t uart_tid;
static k_tid_t adc_tid;

// Definição de prioridades:
#define BLINK_PRIORITY 6
#define UART_PRIORITY  5
#define ADC_PRIORITY   3

// Semáforos para sincronização entre threads:
K_SEM_DEFINE(adc_update_sem, 0, 1);
K_SEM_DEFINE(blink_control_sem, 0, 1);

// Timer para ADC:
K_TIMER_DEFINE(adc_timer, NULL, NULL);

// Verifica disponibilidade do ADC na Device Tree:
#if !DT_NODE_HAS_STATUS(ADC_NODE, okay)
#error "ADC devicetree node is disabled"
#endif

static int16_t adc_sample_buffer[1];

// Configuração do ADC:
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

// Configuração da leitura do ADC:
static const struct adc_sequence adc_seq = {
	.channels = BIT(ADC_CHANNEL_ID),
	.buffer = adc_sample_buffer,
	.buffer_size = sizeof(adc_sample_buffer),
	.resolution = ADC_RESOLUTION,
};

// Variáveis globais para compartilhar dados do ADC entre threads:
static int32_t current_voltage_mv = 0;
static uint8_t current_percentage = 0;
static bool adc_data_ready = false;

// Mutex para proteger acesso aos dados do ADC:
K_MUTEX_DEFINE(adc_data_mutex);

// Fila de mensagens para comunicação UART:
#define MSG_SIZE 16
K_MSGQ_DEFINE(msgq, MSG_SIZE, 10, 4);

const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(usart2));
const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// Estados do LED:
static bool led_on = false;
static bool led_blinking = false;

// Função para obter dados do ADC:
void get_adc_data(int32_t *voltage_mv, uint8_t *percentage)
{
	if (k_mutex_lock(&adc_data_mutex, K_MSEC(100)) == 0) {
		if (adc_data_ready) {
			*voltage_mv = current_voltage_mv;
			*percentage = current_percentage;
		} else {
			*voltage_mv = 0;
			*percentage = 0;
		}
		k_mutex_unlock(&adc_data_mutex);
	}
}

// Callback do timer ADC:
void adc_timer_callback(struct k_timer *timer)
{
	k_sem_give(&adc_update_sem);
}

// Thread para leitura do ADC:
static void adc_thread(void *a, void *b, void *c)
{
	const struct device *adc_dev;
	int32_t ret;
	int32_t voltage_mv;

	adc_dev = DEVICE_DT_GET(ADC_NODE);
	if (!device_is_ready(adc_dev)) {
		printk("ADC device not ready\n");
		return;
	}

	ret = adc_channel_setup(adc_dev, &channel_cfg);
	if (ret < 0) {
		printk("Error setting up ADC channel: %d\n", ret);
		return;
	}

	printk("ADC initialized successfully\n");

	// Inicia o timer para leitura periódica do ADC:
	k_timer_init(&adc_timer, adc_timer_callback, NULL);
	k_timer_start(&adc_timer, K_MSEC(500), K_MSEC(500));

	while (1) {
		k_sem_take(&adc_update_sem, K_FOREVER);

		ret = adc_read(adc_dev, &adc_seq);
		if (ret < 0) {
			printk("ADC read error: %d\n", ret);
			continue;
		}

		voltage_mv = adc_sample_buffer[0];
		ret = adc_raw_to_millivolts(adc_ref_internal(adc_dev), ADC_GAIN, ADC_RESOLUTION,
					    &voltage_mv);

		if (ret < 0) {
			printk("Error converting to mV: %d\n", ret);
			continue;
		}

		uint8_t percentage = (voltage_mv * 100) / 3300;
		if (percentage > 100) {
			percentage = 100;
		}

		if (k_mutex_lock(&adc_data_mutex, K_MSEC(100)) == 0) {
			current_voltage_mv = voltage_mv;
			current_percentage = percentage;
			adc_data_ready = true;
			k_mutex_unlock(&adc_data_mutex);
		}

		printk("ADC: %d mV (%d%%)\n", voltage_mv, percentage);
	}
}

// Função de controle do LED:
void led_control(int command)
{
	switch (command) {
	case 0:
		led_blinking = false;
		led_on = false;
		gpio_pin_set_dt(&led0, 0);
		printk("LED OFF\n");
		break;
	case 1:
		led_blinking = false;
		led_on = true;
		gpio_pin_set_dt(&led0, 1);
		printk("LED ON\n");
		break;
	case 2:
		led_blinking = true;
		led_on = false;
		printk("LED BLINKING\n");
		k_sem_give(&blink_control_sem);
		break;
	default:
		led_blinking = false;
		led_on = false;
		gpio_pin_set_dt(&led0, 0);
		printk("Invalid command! Use: 0=OFF, 1=ON, 2=BLINK\n");
		break;
	}
}

// Funções de informação do sistema:
static void show_thread_info(void)
{
	printk("\n=== THREAD INFORMATION ===\n");
	printk("%-20s %-10s\n", "Thread Name", "Priority");
	printk("------------------------------------------------------------\n");
	printk("%-20s %-10d\n", "main", 0);
	printk("%-20s %-10d\n", "blink_thread", BLINK_PRIORITY);
	printk("%-20s %-10d\n", "uart_thread", UART_PRIORITY);
	printk("%-20s %-10d\n", "adc_thread", ADC_PRIORITY);
	printk("\nSemaphores:\n");
	printk("- adc_update_sem: Controls ADC reading\n");
	printk("- blink_control_sem: Controls LED blinking\n");
	printk("=========================\n\n");
}

static void show_heap_info(void)
{
	struct sys_memory_stats stats;
	sys_heap_runtime_stats_get(&app_heap, &stats);

	printk("\n=== HEAP INFORMATION ===\n");
	printk("Heap Size:       %u bytes\n", APP_HEAP_SIZE);
	printk("Allocated:       %zu bytes\n", APP_HEAP_SIZE - stats.free_bytes);
	printk("Free:            %zu bytes\n", stats.free_bytes);
	printk("========================\n\n");
}

static void show_runtime_info(void)
{
	printk("\n=== RUNTIME INFORMATION ===\n");
	printk("System Uptime: %lld ms\n", k_uptime_get());
	printk("\nThread Status:\n");
	printk("- blink_thread: %s\n", led_blinking ? "Active (blinking)" : "Inactive");
	printk("- uart_thread:  Active (waiting for commands)\n");
	printk("- adc_thread:   Active (timer-driven)\n");
	printk("===========================\n\n");
}

static void show_help(void)
{
	printk("\n=== COMMANDS ===\n");
	printk("LED Control:\n");
	printk("  0 - Turn LED OFF\n");
	printk("  1 - Turn LED ON\n");
	printk("  2 - Start LED BLINKING\n");
	printk("\nSystem Information:\n");
	printk("  info     - Show thread information\n");
	printk("  heap     - Show heap information\n");
	printk("  runtime  - Show runtime information\n");
	printk("  status   - Show current system status\n");
	printk("  help     - Show this help menu\n");
	printk("==========================\n\n");
}

static void show_current_status(void)
{
	int32_t voltage_mv;
	uint8_t percentage;
	get_adc_data(&voltage_mv, &percentage);

	printk("\n=== CURRENT STATUS ===\n");
	printk("LED State: %s\n", led_blinking ? "BLINKING" : (led_on ? "ON" : "OFF"));
	printk("ADC Voltage: %d mV\n", voltage_mv);
	printk("ADC Percentage: %d%%\n", percentage);
	printk("System Uptime: %lld ms\n", k_uptime_get());
	printk("Data Ready: %s\n", adc_data_ready ? "YES" : "NO");
	printk("======================\n\n");
}

// Callback da UART:
static void uart_cb(const struct device *dev, void *user_data)
{
	static char rx_buf[MSG_SIZE];
	static int rx_buf_pos = 0;
	uint8_t c;

	if (!uart_irq_update(uart_dev)) {
		return;
	}

	if (!uart_irq_rx_ready(uart_dev)) {
		return;
	}

	while (uart_fifo_read(uart_dev, &c, 1) == 1) {
		if ((c == '\r' || c == '\n') && rx_buf_pos > 0) {
			rx_buf[rx_buf_pos] = '\0';

			printk("Received command: %s\n", rx_buf);

			if (strlen(rx_buf) == 1 && rx_buf[0] >= '0' && rx_buf[0] <= '2') {
				int led_command = rx_buf[0] - '0';
				led_control(led_command);
			} else if (strcmp(rx_buf, "info") == 0) {
				show_thread_info();
			} else if (strcmp(rx_buf, "heap") == 0) {
				show_heap_info();
			} else if (strcmp(rx_buf, "runtime") == 0) {
				show_runtime_info();
			} else if (strcmp(rx_buf, "help") == 0) {
				show_help();
			} else if (strcmp(rx_buf, "status") == 0) {
				show_current_status();
			} else {
				printk("Unknown command: %s\n", rx_buf);
				printk("Type 'help' for available commands.\n");
			}

			rx_buf_pos = 0;
		} else if (rx_buf_pos < (sizeof(rx_buf) - 1)) {
			rx_buf[rx_buf_pos++] = c;
		}
	}
}

// Thread UART:
static void uart_thread(void *a, void *b, void *c)
{
	char rx_data[MSG_SIZE];
	while (1) {
		k_msgq_get(&msgq, &rx_data, K_FOREVER);
	}
}

// Thread que pisca LED:
static void blink_thread(void *a, void *b, void *c)
{
	static bool blink_state = false;

	while (1) {
		if (led_blinking) {
			blink_state = !blink_state;
			gpio_pin_set_dt(&led0, blink_state ? 1 : 0);
			k_sem_take(&blink_control_sem, K_MSEC(LED_BLINK_INTERVAL_MS));
		} else {
			gpio_pin_set_dt(&led0, led_on ? 1 : 0);
			k_sem_take(&blink_control_sem, K_FOREVER);
		}
	}
}

int main(void)
{
	int ret;

	printk("\n=== STM32 Nucleo-G474RE - LED Control with ADC ===\n");
	printk("LED Commands: 0=OFF, 1=ON, 2=BLINK\n");
	printk("System Commands: info, heap, runtime, status, help\n");
	printk("Enter command: ");

	// Inicializa heap:
	sys_heap_init(&app_heap, app_heap_mem, APP_HEAP_SIZE);

	// Verifica periféricos:
	if (!device_is_ready(led0.port)) {
		printk("ERROR: LED device not ready\n");
		return -1;
	}

	if (!device_is_ready(uart_dev)) {
		printk("ERROR: UART device not ready\n");
		return -1;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		printk("ERROR: Cannot configure LED (%d)\n", ret);
		return ret;
	}

	// Configura UART:
	uart_irq_callback_user_data_set(uart_dev, uart_cb, NULL);
	uart_irq_rx_enable(uart_dev);

	printk("UART initialized successfully\n");

	// Cria threads:
	blink_tid = k_thread_create(&blink_thread_data, blink_thread_stack,
				    K_THREAD_STACK_SIZEOF(blink_thread_stack), blink_thread, NULL,
				    NULL, NULL, BLINK_PRIORITY, 0, K_NO_WAIT);
	k_thread_name_set(blink_tid, "blink_thread");

	uart_tid = k_thread_create(&uart_thread_data, uart_thread_stack,
				   K_THREAD_STACK_SIZEOF(uart_thread_stack), uart_thread, NULL,
				   NULL, NULL, UART_PRIORITY, 0, K_NO_WAIT);
	k_thread_name_set(uart_tid, "uart_thread");

	adc_tid = k_thread_create(&adc_thread_data, adc_thread_stack,
				  K_THREAD_STACK_SIZEOF(adc_thread_stack), adc_thread, NULL, NULL,
				  NULL, ADC_PRIORITY, 0, K_NO_WAIT);
	k_thread_name_set(adc_tid, "adc_thread");

	printk("All threads created successfully\n");
	printk("System ready!\n\n");

	while (1) {
		k_msleep(1000);
	}

	return 0;
}
