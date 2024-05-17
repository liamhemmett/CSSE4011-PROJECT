#include "ir.h"

#define MIN_PERIOD PWM_SEC(1U) / 128U
#define MAX_PERIOD PWM_SEC(1U) / 38000000U

static const struct gpio_dt_spec gpio_dev= GPIO_DT_SPEC_GET(DT_NODELABEL(ir_send), gpios);
static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

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
  gpio_pin_set_dt(&gpio_dev, 1);
  k_sleep(K_MSEC(9));
  gpio_pin_set_dt(&gpio_dev, 0);
  /* Send each byte over IR */
  for (int i = 0; i < 35; i++) {
    ir_transmit(buffer[i]);
  }
}

/* PWM code */

void pwm_init(void) {
    uint32_t max_period;
	uint32_t period;
	uint8_t dir = 0U;
	int ret;

    if (!pwm_is_ready_dt(&pwm_led0)) {
		printk("Error: PWM device %s is not ready\n",
		       pwm_led0.dev->name);
		return;
	}

    max_period = MAX_PERIOD;
	// while (pwm_set_dt(&pwm_led0, max_period, max_period / 2U)) {
	// 	max_period /= 2U;
	// 	if (max_period < (4U * MIN_PERIOD)) {
	// 		printf("Error: PWM device "
	// 		       "does not support a period at least %lu\n",
	// 		       4U * MIN_PERIOD);
	// 		return;
	// 	}
	// }

    period = max_period;
    printf("period = %d\n", period);
    while (1) {
		//ret = pwm_set_dt(&pwm_led0, 1/38000000, 1/76000000);
        ret = pwm_set_dt(&pwm_led0, period, period / 2U);
		if (ret) {
			printk("Error %d: failed to set pulse width\n", ret);
			return;
		}

		period = dir ? (period * 2U) : (period / 2U);
		if (period > max_period) {
			period = max_period / 2U;
			dir = 0U;
		} else if (period < MIN_PERIOD) {
			period = MIN_PERIOD * 2U;
			dir = 1U;
		}

		k_sleep(K_SECONDS(4U));
	}
};