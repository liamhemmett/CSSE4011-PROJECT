#include "timer.h"

/* Create a set of commands. Commands to this set are added using @ref
 * SHELL_SUBCMD_ADD and @ref SHELL_SUBCMD_COND_ADD.
 */
SHELL_SUBCMD_SET_CREATE(sub_section_cmd2, (time));

/* Create a set of subcommands for "section_cmd cm1". */
SHELL_SUBCMD_SET_CREATE(sub_section_cmd3, (time, f));

/* Add command to the set. Subcommand set is identify by parent shell command.
 */

SHELL_SUBCMD_ADD((time), f, &sub_section_cmd3, "Fromatted time", time_f_handler,
                 1, 0);

SHELL_CMD_REGISTER(time, &sub_section_cmd2,
                   "Displays elapsed time since power on", time_handler);

/*
 * Displays time since turned on in seconds
 */
void time_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);
  shell_print(sh, "%lld", k_uptime_get() / 1000);
}

/*
 * Displays time in hr:min:sec since turned on
 */
void time_f_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);
  shell_print(sh, "%lld:%lld:%lld", (k_uptime_get() / (60000 * 60)),
              (k_uptime_get() / 60000), (k_uptime_get() / 1000) % 60);
}