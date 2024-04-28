#include "HCI.h"

/*
 * LEGACY CODE
 * Formats and sends a packer over UART
 */
void send_request(char *cmd, char *data) {
  int len = strlen(data);
  uart_poll_out(uart_pin, PREAMBLE);
  uart_poll_out(uart_pin, REQUEST << 4 | (2 + len));

  uart_poll_out(uart_pin, cmd[0]);
  uart_poll_out(uart_pin, cmd[1]);
  for (int i = 0; i < len; i++) {
    uart_poll_out(uart_pin, data[i]);
  }
  uart_poll_out(uart_pin, END);
}

/*
 * Formats a struct and sends it as a packet over UART
 */
void send_command(char *data) {
  struct packet test;
  int len = strlen(data);
  // Set predefined struct values
  test.preamble = 0xAA;
  test.typeSize = REQUEST << 4 | (2 + len);
  test.end = END;
  // Copies data argument to data field
  for (int i = 0; i < 32; ++i) {
    test.data[i] = data[i];
  }
  if (data[1] != 0xA7) {
    test.data[9] = 0;
  }

  // Converts struct to a str to be sent
  char buffer[sizeof(struct packet) + 1];
  memcpy(buffer, &test, sizeof(struct packet));
  // Send each byte of str
  for (int i = 0; i < 35; i++) {
    uart_poll_out(uart_pin, buffer[i]);
  }
}

/* receive buffer used in UART ISR callback */
static char rx_buf[MSG_SIZE];
static int rx_buf_pos;

/*
 * Read characters from UART until line end is detected. Afterwards push the
 * data to the message queue.
 */
char *serial_cb(const struct device *dev, void *user_data) {
  uint8_t c;

  if (!uart_irq_update(uart_pin)) {
    return NULL;
  }

  if (!uart_irq_rx_ready(uart_pin)) {
    return NULL;
  }

  /* read until FIFO empty */
  while (uart_fifo_read(uart_pin, &c, 1) == 1) {
    if ((c == '\n' || c == '\r') && rx_buf_pos > 0) {
      /* terminate string */
      rx_buf[rx_buf_pos] = '\0';

      /* if queue is full, message is silently dropped */
      // k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);

      /* reset the buffer (it was copied to the msgq) */
      rx_buf_pos = 0;
    } else if (rx_buf_pos < (sizeof(rx_buf) - 1)) {
      rx_buf[rx_buf_pos++] = c;
    }
    /* else: characters beyond buffer size are dropped */
  }
  return NULL;
}