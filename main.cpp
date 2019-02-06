#include <atmel_start.h>
#include <gameduino2/GD2.h>
#include <config/hpl_sercom_config.h>
#include <spi_lite.h>

// Enables USB drive detection and access
//#define ALLOW_USB

static FATFS sdmmc_fatfs;
static FATFS msc_fatfs;

extern void setup(void);
extern void loop(void);

int main(void)
{
	// Initializes MCU, drivers and middleware
	atmel_start_init();

	// Enable SPI
	SPI_0_enable();

	// Give a little blink to show we're running
	gpio_set_pin_level(LED0, false);
	delay_ms(50);
	gpio_set_pin_level(LED0, true);

	// Enter display initialization code
	setup();

	// Run display code loop
	while (1)
		loop();
}

uint8_t spi_xfer_byte(uint8_t send)
{
	return SPI_0_exchange_data(send);
}

int initDisks(void)
{
	dstatus_t status;
	FRESULT result;
	int ret = 0;

	// Begin searching for a connected SD card
	status = STA_NOINIT;
	do {
		status = sdmmc_disk_status(0);
		delay_ms(200);
	} while (status != 0 && status != STA_NODISK);

	if (status == STA_NODISK) {
		printf("SD card not found.\r\n");
		ret = -1;
		return ret;
	} else {
		// SD card found; mount it
		result = f_mount(&sdmmc_fatfs, DRV_SD, 1);
		if (result != FR_OK)
			printf("SD card mounting failed (%d).\r\n", result);
	}

#ifdef ALLOW_USB
	// Search for a USB driver with a 5 second timeout
	status = STA_NOINIT;
	usbhc_start(&USB_HOST_CORE_INSTANCE_inst);
	int tries = 25;
	do {
		status = msc_disk_status(0);
		delay_ms(200);
	} while (status != 0 && status != STA_NODISK && (--tries > 0));

	if (tries <= 0)
		status = STA_NODISK;

	if (status == STA_NODISK) {
		printf("USB drive not found.\r\n");
		ret = 1;
	} else {
		// USB drive found; mount it
		result = f_mount(&msc_fatfs, DRV_USB, 1);
		if (result != FR_OK)
			printf("USB drive mounting failed (%d).\r\n", result);
	}
#endif // ALLOW_USB

	return ret;
}

