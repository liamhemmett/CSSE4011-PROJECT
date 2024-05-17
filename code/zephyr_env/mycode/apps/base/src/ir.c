#include "ir.h"

#define IR DT_ALIAS(ir_send)

static const struct gpio_dt_spec gpio_dev= GPIO_DT_SPEC_GET(DT_NODELABEL(ir_send), gpios);

void gpio_init(void) {
    int ret = 1;
    // Get GPIO device
    if (!gpio_is_ready_dt(&gpio_dev)) {
        printf("Error: GPIO device not found\n");
        return;
    }

    ret = gpio_pin_configure_dt(&gpio_dev, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}
};

/**
 * Reads the bits of payload and blasts IR with correct delays
*/
void ir_transmit(uint8_t data) {
    for (int bit = 0; bit < 8; bit++) {
        if ((data & 0x1 << bit) > 0) {
            /* Send logical 1 using NEC protocol delays */
            gpio_pin_set_dt(&gpio_dev, 1);
            k_sleep(K_USEC(562.2));
            gpio_pin_set_dt(&gpio_dev, 0);
            k_sleep(K_USEC(1687));
        } else {
            /* Send logical 0 using NEC protocol delays */
            gpio_pin_set_dt(&gpio_dev, 1);
            k_sleep(K_USEC(562.2));
            gpio_pin_set_dt(&gpio_dev, 0);
            k_sleep(K_USEC(562.2));
        }
    }
}

/*
 * Formats a struct and sends it as a packet over IR
 */
void send_command(struct packet *payload) {
  // Set predefined struct values
  payload->preambleType = 0xAA;
  payload->end = 0xBB;

  // Converts struct to a str to be sent
  char buffer[sizeof(struct packet) + 1];
  memcpy(buffer, payload, sizeof(struct packet));

  /* Leading Pulse */
  // GPIO HIGH
  k_sleep(K_MSEC(9));
  // GPIO LOW
  /* Send each byte over IR */
  for (int i = 0; i < 35; i++) {
    ir_transmit(buffer[i]);
  }
}