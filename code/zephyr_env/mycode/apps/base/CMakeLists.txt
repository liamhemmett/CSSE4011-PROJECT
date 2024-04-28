# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(AHU)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/timer.c)
target_sources(app PRIVATE src/LED.c)
target_sources(app PRIVATE src/gcu.c)
target_sources(app PRIVATE src/HCI.c)
target_sources(app PRIVATE src/log.c)
target_sources(app PRIVATE src/ble.c)