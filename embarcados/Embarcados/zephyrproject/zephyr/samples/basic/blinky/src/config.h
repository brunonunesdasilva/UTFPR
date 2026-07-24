#ifndef CONFIG_H
#define CONFIG_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/display.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/heap.h>
#include <string.h>
#include <stdio.h>

// Definições do LED (LED2 é o LED verde padrão no Nucleo-G474RE, pino PA5)
#define LED0_NODE DT_ALIAS(led0)

// Definições da UART (USART2 é conectada ao ST-Link no Nucleo)
#define UART_DEVICE_NODE DT_ALIAS(usart2)

// Definições do ADC (ADC1 do STM32G474RE)
#define ADC_NODE             DT_NODELABEL(adc1)
#define ADC_RESOLUTION       12
#define ADC_GAIN             ADC_GAIN_1
#define ADC_REFERENCE        ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME_DEFAULT
#define ADC_CHANNEL_ID       1 // PA0 = ADC1_IN1

// Configurações de temporização
#define LED_BLINK_INTERVAL_MS 500

// Configurações do heap
#define APP_HEAP_SIZE 8192
static uint8_t __aligned(4) app_heap_mem[APP_HEAP_SIZE];
static struct sys_heap app_heap;

// Configurações do display (para SSD1306 via I2C)
#define TEXT_BUFFER_WIDTH  128
#define TEXT_BUFFER_HEIGHT 64
#define TEXT_BUFFER_SIZE   (TEXT_BUFFER_WIDTH * TEXT_BUFFER_HEIGHT)

#endif // CONFIG_H
