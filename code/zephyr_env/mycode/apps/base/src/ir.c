#include "ir.h"

#define IR DT_ALIAS(ir_send)

//static const struct device *gpio_dev;
//static const struct gpio_dt_spec load_switch = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(DT_NODELABEL(load_switch)), gpios, {0});
static const struct gpio_dt_spec gpio_dev= GPIO_DT_SPEC_GET(DT_NODELABEL(ir_send), gpios);
//static const struct gpio_dt_spec gpio_dev= GPIO_DT_SPEC_GET(IR, gpios);
//const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(ir_send));

void gpio_init(void) {
    int ret = 1;
    bool led_state = true;
    printf("1.1\n"); 
    //printf("%s\n", DT_PROP(DT_NODELABEL(ir_send), status)); // 115200
    // Get GPIO device
    if (!gpio_is_ready_dt(&gpio_dev)) {
        printf("Error: GPIO device not found\n");
        return;
    }

    ret = gpio_pin_configure_dt(&gpio_dev, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

    while (1) {
		ret = gpio_pin_toggle_dt(&gpio_dev);
		if (ret < 0) {
			return;
		}

		led_state = !led_state;
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_sleep(K_SECONDS(1));
	}
};

/**
 * Reads the bits of payload and blasts IR with correct delays
*/
void ir_transmit(uint8_t data) {
    for (int bit = 0; bit < 8; bit++) {
        if ((data & 0x1 << bit) > 0) {
            /* Send logical 1 using NEC protocol delays */
            // GPIO HIGH
            k_sleep(K_USEC(562.2));
            // GPIO LOW
            k_sleep(K_USEC(1687));
        } else {
            /* Send logical 0 using NEC protocol delays */
            // GPIO HIGH
            k_sleep(K_USEC(562.2));
            // GPIO LOW
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