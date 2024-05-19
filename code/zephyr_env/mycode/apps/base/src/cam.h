#ifndef CAM_H_
#define CAM_H_

#include <stdio.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

void cam_init(void);
void write_cmd(void);

#ifdef __cplusplus
}
#endif

#endif