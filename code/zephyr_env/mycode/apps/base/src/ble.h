#ifndef BLE_H_
#define BLE_H_

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/slist.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void start_scan(void);
extern int bt_init(void);

#ifdef __cplusplus
}
#endif

#endif