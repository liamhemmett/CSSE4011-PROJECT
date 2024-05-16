#include "cam.h"

/* SPI device name */
//#define SPI_DEV_NAME DT_LABEL(DT_NODELABEL(spi1_default))

/* MOSI pin */
//#define MOSI_PIN DT_PROP_BY_IDX(DT_NODELABEL(spi1_default), mosi_pin, 0)

/* MISO pin */
//#define MISO_PIN DT_PROP_BY_IDX(DT_NODELABEL(spi1_default), miso_pin, 0)

/*void test(void) {
    const struct device *spi_dev;

    // Get SPI device
    spi_dev = device_get_binding(SPI_DEV_NAME);
}*/