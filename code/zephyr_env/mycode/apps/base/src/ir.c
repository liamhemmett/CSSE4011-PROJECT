#include "ir.h"

static const struct device *gpio_dev;
//static struct gpio_dt_spec gpio_dev;


void gpio_init(void) {
    int ret;
    printf("1.1\n"); 
    //gpio_dev = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(arduino_header), gpios, 20);
    gpio_dev = device_get_binding("GPIO_0");
    printf("1.2\n"); 

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