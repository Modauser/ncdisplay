#include <atmel_start.h>

/**
 * Initializes MCU, drivers and middleware in the project
 **/
void atmel_start_init(void)
{
	system_init();
	usb_init();
	sd_mmc_stack_init();
	diskio_init();
	stdio_redirect_init();
}
