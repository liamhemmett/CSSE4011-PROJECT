#ifndef IR_H_
#define IR_H_

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IR_PIN 27
//#define IR_PIN_NODE DT_NODELABEL(IR_PIN)
//static const struct device *const gpio_dev = DEVICE_DT_GET(IR_PIN_NODE);
void gpio_init(void);
void test(void);

#ifdef __cplusplus
}
#endif

#endif