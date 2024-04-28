#include "ble.h"

uint32_t last_write_rate;
struct bt_conn *conn_connected;
static sys_slist_t static_nodes;

SHELL_SUBCMD_SET_CREATE(sub_section_cmd2, (add));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd4, (rmv));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd6, (inspect));

struct node {
  char name[7];
  char mac[18];
  uint16_t major;
  uint16_t minor;
  uint8_t x_coord;
  uint8_t y_coord;
  char left_name[7];
  char right_name[7];
  sys_snode_t next;
};

struct node node_1;
struct node node_2;
struct node node_3;
struct node node_4;
struct node node_5;
struct node node_6;
struct node node_7;
struct node node_8;

/**
 * Handles device discovery and filtering
 */
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
                         struct net_buf_simple *ad) {
  char dev[BT_ADDR_LE_STR_LEN];
  char dev_reduce[BT_ADDR_LE_STR_LEN];

  bt_addr_le_to_str(addr, dev, sizeof(dev));
  strcpy(dev_reduce, dev);
  dev_reduce[17] = '\0';
  if (!strcmp("D4:7E:C0:32:90:82", dev_reduce)) {
    struct node *cur0;
    SYS_SLIST_FOR_EACH_CONTAINER(&static_nodes, cur0, next) {
      if (cur0->name[5] == ad->data[23]) {
        printf("bruv:%02X%02X%02X%02X\n", ad->data[22], ad->data[23],
               ad->data[24], ad->data[25]);
      }
    }
  }
}

/*
 * Initiallisies the scanning procedure
 */
void start_scan(void) {
  int err;

  /* Use active scanning and disable duplicate filtering to handle any
   * devices that might update their advertising data at runtime. */
  struct bt_le_scan_param scan_param = {
      .type = BT_LE_SCAN_TYPE_ACTIVE,
      .options = BT_LE_SCAN_OPT_NONE,
      .interval = BT_GAP_SCAN_FAST_INTERVAL,
      .window = BT_GAP_SCAN_FAST_WINDOW,
  };

  err = bt_le_scan_start(&scan_param, device_found);
  if (err) {
    printf("Scanning failed to start (err %d)\n", err);
    return;
  }

  printf("Scanning successfully started\n");
}

static void init_node_list(void) {
  sys_slist_init(&static_nodes);

  strcpy(node_1.name, "4011-A");
  strcpy(node_1.mac, "F5:75:FE:85:34:67");
  node_1.major = 2753;
  node_1.minor = 32998;
  node_1.x_coord = 0;
  node_1.y_coord = 0;

  strcpy(node_2.name, "4011-B");
  strcpy(node_2.mac, "E5:73:87:06:1E:86");
  node_2.major = 32975;
  node_2.minor = 20959;
  node_2.x_coord = 2;
  node_2.y_coord = 0;

  strcpy(node_3.name, "4011-C");
  strcpy(node_3.mac, "CA:99:9E:FD:98:B1");
  node_3.major = 26679;
  node_3.minor = 40363;
  node_3.x_coord = 4;
  node_3.y_coord = 0;

  strcpy(node_4.name, "4011-D");
  strcpy(node_4.mac, "CB:1B:89:82:FF:FE");
  node_4.major = 41747;
  node_4.minor = 38800;
  node_4.x_coord = 4;
  node_4.y_coord = 2;

  strcpy(node_5.name, "4011-E");
  strcpy(node_5.mac, "D4:D2:A0:A4:5C:AC");
  node_5.major = 30679;
  node_5.minor = 51963;
  node_5.x_coord = 4;
  node_5.y_coord = 4;

  strcpy(node_6.name, "4011-F");
  strcpy(node_6.mac, "C1:13:27:E9:B7:7C");
  node_6.major = 6195;
  node_6.minor = 18394;
  node_6.x_coord = 2;
  node_6.y_coord = 4;

  strcpy(node_7.name, "4011-G");
  strcpy(node_7.mac, "F1:04:48:06:39:A0");
  node_7.major = 30525;
  node_7.minor = 30544;
  node_7.x_coord = 0;
  node_7.y_coord = 4;

  strcpy(node_8.name, "4011-H");
  strcpy(node_8.mac, "CA:0C:E0:DB:CE:60");
  node_8.major = 57395;
  node_8.minor = 28931;
  node_8.x_coord = 0;
  node_8.y_coord = 2;
}

/*
 * Initialises bluetooth
 */
int bt_init(void) {
  init_node_list();
  int err;

  err = bt_enable(NULL);
  if (err) {
    printk("Bluetooth init failed (err %d)\n", err);
    return 0U;
  }
  printk("Bluetooth initialized\n");

  conn_connected = NULL;
  last_write_rate = 0U;
  return 0;
}

/**
 * Updates whole linked list with new neighbours
 */
void update_neighbours(void) {
  struct node *cur1;
  struct node *cur0;
  SYS_SLIST_FOR_EACH_CONTAINER(&static_nodes, cur0, next) {
    char right_node[8];
    int right_mag = 100; // Lowest positive magnitude is right neighbour
    int right_temp = 0;
    char left_node[8];
    int left_mag = 100; // Lowest negative magnitude is left neighbour
    int left_temp = 0;
    SYS_SLIST_FOR_EACH_CONTAINER(&static_nodes, cur1, next) {
      if (strcmp(cur0->name, cur1->name)) {
        // Set initial values
        if (strlen(right_node) == 0) {
          strcpy(right_node, cur1->name);
          strcpy(left_node, cur1->name);
        } else {
          // Get magnitude from target node without negatives
          left_temp = cur0->name[5] - cur1->name[5];
          if (left_temp < 0) {
            left_temp = left_temp + 8;
          }
          right_temp = cur1->name[5] % 8 + cur0->name[5] % 8;
          if (right_temp < cur0->name[5] % 8 * 2) {
            right_temp = right_temp + 8;
          }

          // Check if other ibeacon nodes have smaller magnitudes
          if (right_temp < right_mag) {
            // Save new neighbour and it's magnitude
            right_mag = right_temp;
            strcpy(right_node, cur1->name);
          }
          if (left_temp < left_mag) {
            left_mag = left_temp;
            strcpy(left_node, cur1->name);
          }
        }
      }
    }
    strcpy(cur0->left_name, left_node);
    strcpy(cur0->right_name, right_node);
  }
}

/**
 * Adds the selected node to the linked list
 */
void add_handler(const struct shell *sh, size_t argc, char **argv) {
  // Append the appropriate predefined node
  if (!strcmp(argv[1], "4011-A")) {
    sys_slist_append(&static_nodes, &node_1.next);
  } else if (!strcmp(argv[1], "4011-B")) {
    sys_slist_append(&static_nodes, &node_2.next);
  } else if (!strcmp(argv[1], "4011-C")) {
    sys_slist_append(&static_nodes, &node_3.next);
  } else if (!strcmp(argv[1], "4011-D")) {
    sys_slist_append(&static_nodes, &node_4.next);
  } else if (!strcmp(argv[1], "4011-E")) {
    sys_slist_append(&static_nodes, &node_5.next);
  } else if (!strcmp(argv[1], "4011-F")) {
    sys_slist_append(&static_nodes, &node_6.next);
  } else if (!strcmp(argv[1], "4011-G")) {
    sys_slist_append(&static_nodes, &node_7.next);
  } else if (!strcmp(argv[1], "4011-H")) {
    sys_slist_append(&static_nodes, &node_8.next);
  } else {
    printf("[nrf] [ERR] Not a valid node\n");
  }
  update_neighbours();
}

/**
 * Removes the selected node to the linked list
 */
void rmv_handler(const struct shell *sh, size_t argc, char **argv) {
  // Remove the appropriate predefined node
  if (!strcmp(argv[1], "4011-A")) {
    sys_slist_find_and_remove(&static_nodes, &node_1.next);
  } else if (!strcmp(argv[1], "4011-B")) {
    sys_slist_find_and_remove(&static_nodes, &node_2.next);
  } else if (!strcmp(argv[1], "4011-C")) {
    sys_slist_find_and_remove(&static_nodes, &node_3.next);
  } else if (!strcmp(argv[1], "4011-D")) {
    sys_slist_find_and_remove(&static_nodes, &node_4.next);
  } else if (!strcmp(argv[1], "4011-E")) {
    sys_slist_find_and_remove(&static_nodes, &node_5.next);
  } else if (!strcmp(argv[1], "4011-F")) {
    sys_slist_find_and_remove(&static_nodes, &node_6.next);
  } else if (!strcmp(argv[1], "4011-G")) {
    sys_slist_find_and_remove(&static_nodes, &node_7.next);
  } else if (!strcmp(argv[1], "4011-H")) {
    sys_slist_find_and_remove(&static_nodes, &node_8.next);
  } else {
    printf("[nrf] [ERR] Not a valid node\n");
  }
  update_neighbours();
}

/**
 * Displays all ibeacon node details for a specified node name, or all nodes if
 * -a is included
 */
void inspect_handler(const struct shell *sh, size_t argc, char **argv) {
  struct node *cur;
  SYS_SLIST_FOR_EACH_CONTAINER(&static_nodes, cur, next) {
    if (!strcmp(argv[1], "-a")) {
      printf("[nrf] Name: %s, BLE Addr: %s, Major: %d, Minor: %d, Coord: [%d, "
             "%d], "
             "Left Node: %s, Right Node: %s\n",
             cur->name, cur->mac, cur->major, cur->minor, cur->x_coord,
             cur->y_coord, cur->left_name, cur->right_name);
    } else if (!strcmp(argv[1], cur->name)) {
      printf("[nrf] Name: %s, BLE Addr: %s, Major: %d, Minor: %d, Coord: [%d, "
             "%d], "
             "Left Node: %s, Right Node: %s\n",
             cur->name, cur->mac, cur->major, cur->minor, cur->x_coord,
             cur->y_coord, cur->left_name, cur->right_name);
    }
  }
}

SHELL_SUBCMD_SET_CREATE(sub_section_cmd1, (add, name, mac, maj, min, x, y));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd3, (rmv, name));
SHELL_SUBCMD_SET_CREATE(sub_section_cmd5, (inspect, a));

SHELL_CMD_REGISTER(add, &sub_section_cmd2, "Add an ibeacon node", add_handler);
SHELL_CMD_REGISTER(rmv, &sub_section_cmd4, "Remove an ibeacon node",
                   rmv_handler);
SHELL_CMD_REGISTER(inspect, &sub_section_cmd6,
                   "View an ibeacon node details (use -a to view all)",
                   inspect_handler);