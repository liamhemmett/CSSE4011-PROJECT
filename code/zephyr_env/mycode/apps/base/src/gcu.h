#ifndef GCU_H_
#define GCU_H_

#include <stdlib.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#include "HCI.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GRAPH 1
#define METER 2
#define NUMERIC 3
#define TEMP 5
#define HUMID 6
#define AIR 7
#define TVOC 8
#define SD 13

struct sensorPlot {
  __uint8_t sensor;
  __uint8_t display;
};

void point_handler(const struct shell *sh, size_t argc, char **argv);
void circle_handler(const struct shell *sh, size_t argc, char **argv);
void lj_handler(const struct shell *sh, size_t argc, char **argv);
void dac_handler(const struct shell *sh, size_t argc, char **argv);
void gled_handler(const struct shell *sh, size_t argc, char **argv);
void gpd_handler(const struct shell *sh, size_t argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif