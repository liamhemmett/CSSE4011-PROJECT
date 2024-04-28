#include "gcu.h"
#include "log.h"

/* Create a set of commands. Commands to this set are added using @ref
 * SHELL_SUBCMD_ADD and @ref SHELL_SUBCMD_COND_ADD.
 */
SHELL_SUBCMD_SET_CREATE(sub_section_cmd4, (point));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd6, (circle));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd8, (lj));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd10, (dac));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd12, (gled));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd14, (gpb));
// SHELL_SUBCMD_SET_CREATE(sub_section_cmd16, (blescan));
// SHELL_SUBCMD_SET_CREATE(sub_section_cmd18, (blecon));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd20, (gcugraph));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd22, (gcumeter));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd24, (gcunumeric));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd26, (gcurec));

struct sensorPlot flags;

/*
 * Creates point at specified coordinate
 */
void point_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  if (check_command(argc, argv)) {
    return;
  }
  char buffer[1024];
  char x[4];
  char y[4];
  /* If input number is single digit, add .0 to string */
  if (!strchr(argv[1], '.')) {
    snprintf(x, 4, "%c.0", argv[1][0]);
  } else {
    strcpy(x, argv[1]);
  }
  if (!strchr(argv[2], '.')) {
    snprintf(y, 4, "%c.0", argv[2][0]);
  } else {
    strcpy(y, argv[2]);
  }
  snprintf(buffer, sizeof(buffer), "%c%c%c%c%c%c", x[0], x[1], x[2], y[0], y[1],
           y[2]);
  send_request("PO", buffer);
}

/*
 * Creates circle at specified coordinate
 */
void circle_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  if (check_command(argc, argv)) {
    return;
  }
  char buffer[1024];
  char x[4];
  char y[4];
  char r[4];
  /* If input number is single digit, add .0 to string */
  if (!strchr(argv[1], '.')) {
    snprintf(r, 4, "%c.0", argv[1][0]);
  } else {
    strcpy(r, argv[1]);
  }
  if (!strchr(argv[2], '.')) {
    snprintf(x, 4, "%c.0", argv[2][0]);
  } else {
    strcpy(x, argv[2]);
  }
  if (!strchr(argv[3], '.')) {
    snprintf(y, 4, "%c.0", argv[3][0]);
  } else {
    strcpy(y, argv[3]);
  }
  snprintf(buffer, sizeof(buffer), "%c%c%c%c%c%c%c%c%c", r[0], r[1], r[2], x[0],
           x[1], x[2], y[0], y[1], y[2]);
  send_request("CI", buffer);
}

/*
 * Create curve at specified coordinate
 */
void lj_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  if (check_command(argc, argv)) {
    return;
  }
  char buffer[1024];
  char a[4];
  char b[4];
  char t[4];
  /* If input number is single digit, add .0 to string */
  if (!strchr(argv[1], '.')) {
    snprintf(a, 4, "%c.0", argv[1][0]);
  } else {
    strcpy(a, argv[1]);
  }
  if (!strchr(argv[2], '.')) {
    snprintf(b, 4, "%c.0", argv[2][0]);
  } else {
    strcpy(b, argv[2]);
  }
  if (!strchr(argv[3], '.')) {
    snprintf(t, 4, "%c.0", argv[3][0]);
  } else {
    strcpy(t, argv[3]);
  }
  snprintf(buffer, sizeof(buffer), "%c%c%c%c%c%c%c%c%c", a[0], a[1], a[2], b[0],
           b[1], b[2], t[0], t[1], t[2]);
  send_request("LJ", buffer);
}

/*
 * Sets dac to a specific voltage
 */
void dac_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  if (check_command(argc, argv)) {
    return;
  }
  char buffer[1024];
  char x[4];
  char y[4];
  /* If input number is single digit, add .0 to string */
  if (!strchr(argv[1], '.')) {
    snprintf(x, 4, "%c.0", argv[1][0]);
  } else {
    strcpy(x, argv[1]);
  }
  if (!strchr(argv[2], '.')) {
    snprintf(y, 4, "%c.0", argv[2][0]);
  } else {
    strcpy(y, argv[2]);
  }
  snprintf(buffer, sizeof(buffer), "%c%c%c%c%c%c", x[0], x[1], x[2], y[0], y[1],
           y[2]);
  send_request("DA", buffer);
}

/*
 * Toggles the GCUs LEDs
 */
void gled_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  if (check_command(argc, argv)) {
    return;
  }
  char buffer[1024];
  char r[4];
  char g[4];
  char b[4];
  /* If input number is single digit, add .0 to string */
  if (!strchr(argv[1], '.')) {
    snprintf(r, 4, "%c.0", argv[1][0]);
  } else {
    strcpy(r, argv[1]);
  }
  if (!strchr(argv[2], '.')) {
    snprintf(g, 4, "%c.0", argv[2][0]);
  } else {
    strcpy(g, argv[2]);
  }
  if (!strchr(argv[3], '.')) {
    snprintf(b, 4, "%c.0", argv[3][0]);
  } else {
    strcpy(b, argv[3]);
  }
  snprintf(buffer, sizeof(buffer), "%c%c%c%c%c%c%c%c%c", r[0], r[1], r[2], g[0],
           g[1], g[2], b[0], b[1], b[2]);
  send_request("GL", buffer);
}

/*
 * Requests for push button status on GCU
 */
void gpd_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  if (check_command(argc, argv)) {
    return;
  }
  char buffer[1024];
  send_request("GP", buffer);
}

/*
 * Scans for bluetooth connections with a filtered address
 */
void blescan_f_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);
  shell_print(sh, "ble f\n");
}
/*
 * Scans for all bluetooth connections
 */
void blescan_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);
  shell_print(sh, "ble non f\n");
}

/*
 * Commences recieval of sensor data from connected device
 */
void blecon_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);
  shell_print(sh, "blecon p\n");
}

/*
 * Halts data flow from connected device
 */
void blecon_s_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);
  shell_print(sh, "blecon p\n");
}

/*
 * Informs GCU to plot the sensor data to a graph
 */
void gcugraph_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  ARG_UNUSED(argc);
  char buffer[32] = {0};
  flags.display = GRAPH;
  buffer[0] = GRAPH;
  // Add sensor device to data field
  if (!strcmp("Temp", argv[1])) {
    flags.sensor = TEMP;
    buffer[1] = TEMP;
  } else if (!strcmp("Humidity", argv[1])) {
    flags.sensor = HUMID;
    buffer[1] = HUMID;
  } else if (!strcmp("Pressure", argv[1])) {
    flags.sensor = AIR;
    buffer[1] = AIR;
  } else if (!strcmp("TVOC", argv[1])) {
    flags.sensor = TVOC;
    buffer[1] = TVOC;
  }
  send_command(buffer);
}

/*
 * Informs the GCU to plot the sensor data as a meter
 */
void gcumeter_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  ARG_UNUSED(argc);
  char buffer[32] = {0};
  flags.display = METER;
  buffer[0] = METER;
  // Add sensor device to data field
  if (!strcmp("Temp", argv[1])) {
    flags.sensor = TEMP;
    buffer[1] = TEMP;
  } else if (!strcmp("Humidity", argv[1])) {
    flags.sensor = HUMID;
    buffer[1] = HUMID;
  } else if (!strcmp("Pressure", argv[1])) {
    flags.sensor = AIR;
    buffer[1] = AIR;
  } else if (!strcmp("TVOC", argv[1])) {
    flags.sensor = TVOC;
    buffer[1] = TVOC;
  }
  send_command(buffer);
}

/*
 * Informs the GCU to plot the sensor data as numerical
 */
void gcunumeric_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  ARG_UNUSED(argc);
  char buffer[32] = {0};
  flags.display = NUMERIC;
  buffer[0] = NUMERIC;
  // Add sensor device to data field
  if (!strcmp("Temp", argv[1])) {
    flags.sensor = TEMP;
    buffer[1] = TEMP;
  } else if (!strcmp("Humidity", argv[1])) {
    flags.sensor = HUMID;
    buffer[1] = HUMID;
  } else if (!strcmp("Pressure", argv[1])) {
    flags.sensor = AIR;
    buffer[1] = AIR;
  } else if (!strcmp("TVOC", argv[1])) {
    flags.sensor = TVOC;
    buffer[1] = TVOC;
  }
  send_command(buffer);
}

/*
 * Tells GCU to save sensor data to a file name specified in arguments
 */
void gcurec_handler(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(sh);
  ARG_UNUSED(argc);
  char buffer[32] = {0};
  buffer[0] = SD;
  flags.display = SD;
  // Add sensor device to data field
  if (!strcmp("-s", argv[1])) {
    buffer[1] = 0xA7;
    flags.sensor = 0xA9;
  } else if (!strcmp("-p", argv[1])) {
    buffer[1] = 0xA8;
    flags.sensor = 0xA8;
  }
  // Saves file name to appropriate location in buffer
  for (int i = 0; i < strlen(argv[3]) + 1; i++) {
    buffer[i + 2] = argv[3][i];
  }
  send_command(buffer);
}

/* Create a set of subcommands for "section_cmd cm1". */
SHELL_SUBCMD_SET_CREATE(sub_section_cmd5, (point, x, y));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd7, (circle, r, x, y));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd9, (lj, a, b, t));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd11, (dac, x, y));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd13, (gled, r, g, b));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd15, (gpb));
// SHELL_SUBCMD_SET_CREATE(sub_section_cmd17, (blescan, s, f, addr));
// SHELL_SUBCMD_SET_CREATE(sub_section_cmd19, (blecon, s, addr));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd21, (gcugraph, id));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd23, (gcumeter, id));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd25, (gcunumeric, id));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd27, (gcurec, s, id, name));

/* Add command to the set. Subcommand set is identify by parent shell command.
 */
// SHELL_SUBCMD_ADD((blescan), f, &sub_section_cmd17, "Filtered BLE scan",
// blescan_f_handler, 1, 0); SHELL_SUBCMD_ADD((blecon), addr,
// &sub_section_cmd19, "Start recieving", blecon_handler, 1, 0);

SHELL_CMD_REGISTER(point, &sub_section_cmd4, "Draw a point at [x, y]",
                   point_handler);
SHELL_CMD_REGISTER(circle, &sub_section_cmd6,
                   "Draw a circle with radius r with a center at [x,y]",
                   circle_handler);
SHELL_CMD_REGISTER(lj, &sub_section_cmd8,
                   "Set the a and b parameters for the Lissajous curve",
                   lj_handler);
SHELL_CMD_REGISTER(dac, &sub_section_cmd10,
                   "Set voltage values on the X and Y DACs", dac_handler);
SHELL_CMD_REGISTER(gled, &sub_section_cmd12, "Toggle the GCU (c) User LEDs",
                   gled_handler);
SHELL_CMD_REGISTER(gpd, &sub_section_cmd14,
                   "Read GCU pushbutton state (0 or 1)", gpd_handler);
// SHELL_CMD_REGISTER(blescan, &sub_section_cmd16,
//				   "Scan for bluetooth devices",
// blescan_handler); SHELL_CMD_REGISTER(blecon, &sub_section_cmd18,
// "Recieves sensor data from selected device", blecon_s_handler);
SHELL_CMD_REGISTER(gcugraph, &sub_section_cmd20,
                   "Displays selected sensor values as a graph plot",
                   gcugraph_handler);
SHELL_CMD_REGISTER(gcumeter, &sub_section_cmd22,
                   "Displays selected sensor values as a meter widget",
                   gcumeter_handler);
SHELL_CMD_REGISTER(gcunumeric, &sub_section_cmd24,
                   "Displays selected sensor values as a numeric value",
                   gcunumeric_handler);
SHELL_CMD_REGISTER(gcurec, &sub_section_cmd26,
                   "Start continuously recording the selected sensor values",
                   gcurec_handler);
