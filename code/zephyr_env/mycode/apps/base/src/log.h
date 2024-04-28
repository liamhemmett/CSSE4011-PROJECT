#ifndef LOG_H_
#define LOG_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/logging/log_ctrl.h>
#include <zephyr/logging/log_output.h>
#include <zephyr/shell/shell.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_MODULE_NAME syst
#define ERR "invalid command"

int check_num(char *str);
int check_command(size_t argc, char **command);
void show_state(int ledId, int state, int prevState);

#ifdef __cplusplus
}
#endif

#endif