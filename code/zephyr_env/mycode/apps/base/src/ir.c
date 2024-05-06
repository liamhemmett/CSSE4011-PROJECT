#include "ir.h"

static const struct device *gpio_dev;
static const struct gpio_dt_spec load_switch = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(DT_NODELABEL(load_switch)), gpios, {0});
//static struct gpio_dt_spec gpio_dev;


void gpio_init(void) {
    int ret;
    printf("1.1\n"); 
    gpio_dev = device_get_binding("GPIO_0");
    
    //gpio_dev = device_get_binding(DEVICE_DT_GET_ANY(arduino-header-r3));
    printf("1.2\n"); 

    if (!gpio_is_ready_dt(&load_switch)) {
        //printf("%s\n", DT_PROP(DT_NODELABEL(load_switch), status));
		printf("The load switch pin GPIO port is not ready.\n");
		return;
	}

    // Get GPIO device
    if (!gpio_dev) {
        printf("Error: GPIO device not found\n");
        return;
    }

    // Configure GPIO pin
    ret = gpio_pin_configure(gpio_dev, IR_PIN, GPIO_OUTPUT); // or GPIO_INPUT
    printf("1.3\n"); 

    //ret = gpio_pin_configure_dt(&gpio_dev, GPIO_OUTPUT); // or GPIO_INPUT
    if (ret < 0) {
        printf("Error: Failed to configure GPIO pin\n");
        return;
    }
};

void test(void) {
    int ret;
    // Turn on the GPIO pin
    ret = gpio_pin_set(gpio_dev, IR_PIN, 0);
    //ret = gpio_pin_set_dt(&gpio_dev, 1);
    if (ret < 0) {
        printf("Error: Failed to set GPIO pin\n");
        return;
    }

    // Delay for 1 second
    k_sleep(K_SECONDS(1));

    // Turn off the GPIO pin
    ret = gpio_pin_set(gpio_dev, IR_PIN, 1);
    //ret = gpio_pin_set_dt(&gpio_dev, 0);
    if (ret < 0) {
        printf("Error: Failed to set GPIO pin\n");
        return;
    }
};

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