#include "log.h"

LOG_MODULE_REGISTER(LOG_MODULE_NAME, LOG_LEVEL_DBG);

/*
 * Displays states of LEDs with consideration of past values
 */
void show_state(int ledId, int state, int prevState) {
  if (state != prevState || prevState == -1) {
    if (state) {
      LOG_INF("LED%d is on", ledId);
    } else {
      LOG_INF("LED%d is off", ledId);
    }
  }
  if (state == prevState) {
    if (state) {
      LOG_WRN("LED%d is already on", ledId);
    } else {
      LOG_WRN("LED%d is already off", ledId);
    }
  }
}

/*
 * Checks arg is valid number. Returns 1 if error
 */
int check_num(char *str) {
  int p_count = 0;

  for (int i = 0; str[i] != '\0'; i++) {
    // Check for alpha characters
    if (isalpha(str[i]) != 0) {
      LOG_ERR(ERR);
      return 1;
    }

    if (str[i] == '.') {
      p_count++;
      if (p_count > 1) {
        LOG_ERR(ERR);
        return 1;
      }
    }
  }
  // Check for period count
  if (p_count == 0) {
    if (strlen(str) > 1) {
      LOG_ERR(ERR);
      return 1;
    }
  }
  return 0;
}

/*
 * Check if a shell command is valid
 * return 0 if valid, 1 if invalid
 */
int check_command(size_t argc, char **command) {
  // Check for valid command amount
  if (!strcmp("point", command[0]) || !strcmp("dac", command[0]) ||
      !strcmp("led", command[0])) {
    if (argc != 3) {
      LOG_ERR(ERR);
      return 1;
    }
    // Checks number arguments for GCU comms
    if (strcmp("led", command[0])) {
      if (!check_num(command[1]) && !check_num(command[2])) {
        return 0;
      } else {
        return 1;
      }
    } else {
      // Check arguments for LED
      if (strlen(command[1]) == 1 && isalpha(command[1][0])) {
        return check_num(command[2]);
      } else {
        LOG_ERR(ERR);
        return 1;
      }
    }
  } else if (!strcmp("circle", command[0]) || !strcmp("lj", command[0]) ||
             !strcmp("gled", command[0])) { // Check correct arg lengths
    if (argc != 4) {
      LOG_ERR(ERR);
      return 1;
    }
    if (!check_num(command[1]) && !check_num(command[2]) &&
        !check_num(command[3])) { // Check valid types
      return 0;
    } else {
      return 1;
    }
  } else if (!strcmp("gpd", command[0])) {
    if (argc != 1) { // Check correct arg lengths
      LOG_ERR(ERR);
      return 1;
    }
  }
  return 0;
}