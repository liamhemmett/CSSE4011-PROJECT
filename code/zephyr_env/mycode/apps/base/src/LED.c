#include "LED.h"
#include "log.h"

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);
static const struct gpio_dt_spec LEDs[] = {led0, led1, led2, led3};

/* Create a set of commands. Commands to this set are added using @ref
 * SHELL_SUBCMD_ADD and @ref SHELL_SUBCMD_COND_ADD.
 */
SHELL_SUBCMD_SET_CREATE(sub_section_cmd0, (led));
/* Create a set of subcommands for "led". */
SHELL_SUBCMD_SET_CREATE(sub_section_cmd1, (led, s, t));

/* Add command to the set. Subcommand set is identify by parent shell command.
 */
SHELL_SUBCMD_ADD((led), s, &sub_section_cmd1, "Switch", switch_handler, 2, 0);
SHELL_SUBCMD_ADD((led), t, &sub_section_cmd1, "Toggle", toggle_handler, 2, 0);

SHELL_CMD_REGISTER(led, &sub_section_cmd0, "Control board LEDs", NULL);

/*
 * Switches LED to specified state
 */
void switch_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  check_command(argc, argv);
  static int prev[4] = {0, 0, 0, 0};

  for (int i = 0; i < 4; i++) {
    if (argv[1][i] == 48) {
      gpio_pin_set_dt(&LEDs[i], 0);
      show_state(i, 0, prev[i]);
      prev[i] = 0;
    } else if (argv[1][i] == 49) {
      gpio_pin_set_dt(&LEDs[i], 1);
      show_state(i, 1, prev[i]);
      prev[i] = 1;
    }
  }
}

/*
 * Toggles specified LED
 */
void toggle_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  check_command(argc, argv);
  int state = 0;

  for (int i = 0; i < 4; i++) {
    if (argv[1][i] == 49) {
      gpio_pin_toggle_dt(&LEDs[i]);
      state = gpio_pin_get_dt(&LEDs[i]);
      show_state(i, state, -1);
    }
  }
}

int LED_init(void) {
  // Init LEDs
  if (!gpio_is_ready_dt(&led0) || !gpio_is_ready_dt(&led1) ||
      !gpio_is_ready_dt(&led2) || !gpio_is_ready_dt(&led3)) {
    return 0;
  }

  if (gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE) < 0 ||
      gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE) < 0 ||
      gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE) < 0 ||
      gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE) < 0) {
    return 0;
  }
  return 1;
}