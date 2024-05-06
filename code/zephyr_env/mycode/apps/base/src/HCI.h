#ifndef HCI_H_
#define HCI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UART_SHELL_NODE DT_CHOSEN(zephyr_shell_uart)
#define UART_PIN_NODE DT_NODELABEL(uart1)

#define PREAMBLE 0xAA
#define REQUEST 0x01
#define RESPONSE 0x02
#define END 0x55

#define LED_DID 1
#define PB_DID 2
#define DAC 3
#define LCP 4
#define TEMP 5
#define HUMID 6
#define AIR 7
#define TVOC 8
#define SD 13

#define MSG_SIZE 32

static const struct device *const uart_dev = DEVICE_DT_GET(UART_SHELL_NODE);
static const struct device *const uart_pin = DEVICE_DT_GET(UART_PIN_NODE);

extern struct packet {
  __uint8_t preamble;
  __uint8_t typeSize;
  __uint8_t data[32];
  __uint8_t end;
};

extern void send_request(char *cmd, char *data);
void send_command(char *data);
char *serial_cb(const struct device *dev, void *user_data);

#ifdef __cplusplus
}
#endif

#endif