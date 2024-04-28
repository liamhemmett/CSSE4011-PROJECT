#ifndef LED_H_
#define LED_H_

#include <stdlib.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#ifdef __cplusplus
extern "C" {
#endif

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

void switch_handler(const struct shell *sh, size_t argc, char **argv);
void toggle_handler(const struct shell *sh, size_t argc, char **argv);
void led_handler(const struct shell *sh, size_t argc, char **argv);
extern int LED_init(void);

#ifdef __cplusplus
}
#endif

#endif