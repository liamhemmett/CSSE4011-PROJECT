#include "cam.h"

#define SPI_OP  SPI_OP_MODE_MASTER |SPI_MODE_CPOL | SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_LINES_SINGLE
#define BUF_SIZE 32

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

    const struct spi_dt_spec cam_dev =
                SPI_DT_SPEC_GET(DT_NODELABEL(camera), SPI_OP, 0);

    ret = spi_read_dt(&cam_dev, &rx);
    if (ret) { printf("spi_read status: %d", ret); }
}