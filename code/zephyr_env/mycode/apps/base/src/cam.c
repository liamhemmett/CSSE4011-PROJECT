#include "cam.h"

#define SPI_OP  SPI_OP_MODE_MASTER |SPI_MODE_CPOL | SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_LINES_SINGLE
#define BUF_SIZE 32

static const struct gpio_dt_spec gpio_dev= GPIO_DT_SPEC_GET(DT_NODELABEL(cs_pin), gpios);
const struct spi_dt_spec cam_dev = SPI_DT_SPEC_GET(DT_NODELABEL(camera), SPI_OP, 0);

static __aligned(32) char buffer_tx[BUF_SIZE];
static __aligned(32) char buffer_rx[BUF_SIZE];


const struct spi_buf tx_bufs[] = {
	/* According to documentation, when sending NULL tx buf -
	 *  NOP frames should be sent on MOSI line
	 */
	{
		.buf = buffer_tx,
		.len = BUF_SIZE,
	},
};
const struct spi_buf rx_bufs[] = {
	{
		.buf = buffer_rx,
		.len = BUF_SIZE,
	},
};
const struct spi_buf_set tx = {
	.buffers = tx_bufs,
	.count = ARRAY_SIZE(tx_bufs)
};
const struct spi_buf_set rx = {
	.buffers = rx_bufs,
	.count = ARRAY_SIZE(rx_bufs)
};

void cam_init(void) {
    int ret;
	printf("Camera Initialising\n");

    /* Setup CS Pin */
    if (!gpio_is_ready_dt(&gpio_dev)) {
        printf("Error: GPIO device not found\n");
        return;
    }

    ret = gpio_pin_configure_dt(&gpio_dev, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}
	gpio_pin_set_dt(&gpio_dev, 1);

	/* Setup SPI pins*/

    ret = spi_read_dt(&cam_dev, &rx);
    if (ret) { printf("spi_read status: %d", ret); }
	/* Turn camera on */
	memset(buffer_tx, 0x0205, BUF_SIZE);
	gpio_pin_set_dt(&gpio_dev, 0);
	ret = spi_write_dt(&cam_dev, &tx);
	if (ret) { printf("spi_read status: %d", ret); }
	gpio_pin_set_dt(&gpio_dev, 1);
	k_sleep(K_MSEC(10));
	/* Set resolution */
	memset(buffer_tx, 0x2101, BUF_SIZE);
	gpio_pin_set_dt(&gpio_dev, 0);
	spi_write_dt(&cam_dev, &tx);
	gpio_pin_set_dt(&gpio_dev, 1);
	printf("Camera Initialised\n");
}

void write_cmd(void) {
	int ret;
	memset(buffer_tx, 0x0681, BUF_SIZE);
	gpio_pin_set_dt(&gpio_dev, 0);
	spi_write_dt(&cam_dev, &tx);
	gpio_pin_set_dt(&gpio_dev, 1);

	gpio_pin_set_dt(&gpio_dev, 0);
	ret = spi_read_dt(&cam_dev, &rx);
	if (ret) { printf("spi_read status: %d", ret); }
	k_sleep(K_MSEC(1000));
	gpio_pin_set_dt(&gpio_dev, 1);


	printf("trying to read: %x\n", (int)buffer_rx);
}