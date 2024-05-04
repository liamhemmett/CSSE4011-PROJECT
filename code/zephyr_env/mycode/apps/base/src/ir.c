#include "ir.h"

struct device *gpio_dev;
//

void gpio_init(void) {
    int ret;
    gpio_dev = GPIO_DT_SPEC_GET(DT_NODELABEL(ir_trans), gpios);

    // Get GPIO device
    if (!gpio_dev) {
        printk("Error: GPIO device not found\n");
        return;
    }

    // Configure GPIO pin
    ret = gpio_pin_configure(gpio_dev, IR_PIN, GPIO_OUTPUT); // or GPIO_INPUT
    if (ret < 0) {
        printk("Error: Failed to configure GPIO pin\n");
        return;
    }
};

void test(void) {
    int ret;
    // Turn on the GPIO pin
    ret = gpio_pin_set(gpio_dev, IR_PIN, 1);
    if (ret < 0) {
        printk("Error: Failed to set GPIO pin\n");
        return;
    }

    // Delay for 1 second
    k_sleep(K_SECONDS(1));

    // Turn off the GPIO pin
    ret = gpio_pin_set(gpio_dev, IR_PIN, 0);
    if (ret < 0) {
        printk("Error: Failed to set GPIO pin\n");
        return;
    }
};