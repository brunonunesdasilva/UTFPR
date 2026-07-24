#ifndef CONFIG_H
#define CONFIG_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <string.h>
#include <stdio.h>

/* ========================================================================== */
/* LED DEFINITIONS */
/* ========================================================================== */
#if DT_NODE_HAS_STATUS(DT_ALIAS(led0), okay)
#define LED0_NODE DT_ALIAS(led0)
#elif DT_NODE_HAS_STATUS(DT_ALIAS(led1), okay)
#define LED0_NODE DT_ALIAS(led1)
#elif DT_NODE_HAS_STATUS(DT_ALIAS(led2), okay)
#define LED0_NODE DT_ALIAS(led2)
#else
#warning "Nenhum LED encontrado"
#endif

/* ========================================================================== */
/* PWM SERVO DEFINITIONS */
/* ========================================================================== */
// #define PWM_SERVO_NODE DT_PWMS_CTLR(PWM_SERVO_NODE)

// #if !DT_NODE_EXISTS(PWM_SERVO_NODE)
// #error "pwm-servo alias not found in devicetree"
// #endif

// #define PWM_SERVO_CTLR DT_PARENT(PWM_SERVO_NODE)

/* ========================================================================== */
/* SERVO PARAMETERS */
/* ========================================================================== */
#define SERVO_MIN_PULSE_US 500
#define SERVO_MAX_PULSE_US 2500
#define SERVO_PERIOD_US 20000

/* ========================================================================== */
/* ADC DEFINITIONS */
/* ========================================================================== */
#define ADC_NODE DT_NODELABEL(adc1)
#define ADC_CHANNEL_ID 1
#define ADC_RESOLUTION 12
#define ADC_GAIN ADC_GAIN_1
#define ADC_REFERENCE ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME_DEFAULT
#define ADC_MAX_VALUE 4095

/* ========================================================================== */
/* TIMING */
/* ========================================================================== */
#define LED_BLINK_INTERVAL_MS 500
#define ADC_READ_INTERVAL_MS 50
#define SERVO_UPDATE_INTERVAL_MS 20

/* ========================================================================== */
/* HEAP */
/* ========================================================================== */
#define APP_HEAP_SIZE 8192
static uint8_t __aligned(4) app_heap_mem[APP_HEAP_SIZE];
static struct sys_heap app_heap;

#endif // CONFIG_H
