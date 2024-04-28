#ifndef TIMER_H_
#define TIMER_H_

#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#ifdef __cplusplus
extern "C" {
#endif

void time_handler(const struct shell *sh, size_t argc, char **argv);
void time_f_handler(const struct shell *sh, size_t argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif