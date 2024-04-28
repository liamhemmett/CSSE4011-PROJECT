PRAC 3 Localisation with Data Fusion - Application Host Unit

Marco Day
46406235

Firmware for base node. Base node recieves RSSI and node values from mobile node and
relays them to a GUI. Shell commands enable specific static nodes for distance measurements

References:
~/zephyr/samples
~/zephyr/samples/bluetooth/central_gatt_write
~/zephyr/samples/bluetooth/central_hr
~/zephyr/samples/basic/blinky
~/zephyr/samples/subsys/shell/shell_module
~/zephyr/samples/drivers/uart/echo_bot

Instructions:
west build -b nrf52840dk_nrf52840 --pristine
west flash --recover

screen /dev/ttyACM0 115200
