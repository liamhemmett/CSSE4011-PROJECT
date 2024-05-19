#ifndef IR_H_
#define IR_H_

#include <stdio.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IR_PIN 27

struct packet {
  __uint8_t preambleType;
  __uint8_t data[32];
  __uint8_t end;
};

void gpio_init(void);
void ir_transmit(uint8_t data);
void send_command(struct packet *payload);
void pwm_init(void);

#ifdef __cplusplus
}
#endif

#endif