#include <atmel_start.h>
#include <gameduino2/GD2.h>
#include <config/hpl_sercom_config.h>
#include <spi_lite.h>

//extern void setup(void);
//extern void loop(void);

constexpr uint32_t NCIdentifier = 0x31415926;
static uint32_t *ncident = (uint32_t *)0x00080000;
static void (*ncmain)(void) = (void (*)(void))(0x00080004 | 1);

void UsageFault_Handler(void)
{
	while (1);
}

void HardFault_Handler(void)
{
	while (1);
}

int main(void)
{
	// Initializes MCU, drivers and middleware
	atmel_start_init();

	// Enable SPI
	SPI_0_enable();

	// Confirm existance of display code, then jump to it
	if (*ncident == NCIdentifier)
		ncmain();

	while (1) {
		gpio_set_pin_level(LED0, false);
		delay_ms(750);
		gpio_set_pin_level(LED0, true);
		delay_ms(250);
	}
}

uint8_t spi_xfer_byte(uint8_t send)
{
	return SPI_0_exchange_data(send);
}

uint32_t diskio_svc_handler(int n, uint32_t *diskio_args);
void firmware_update(uint32_t addr, uint32_t count);
void SVCall_Handler(void)
{
	uint32_t *args;

	asm("\
		tst lr, #4; \
		ite eq; \
		mrseq %0, msp; \
		mrsne %0, psp; \
	" : "=r" (args));

	int svc_number = ((char *)args[6])[-2];
	switch (svc_number) {
	case 0:
		delay_ms(args[1]);
		break;
	case 1:
		*((uint32_t *)args[0]) = spi_xfer_byte(args[1]);
		break;
	case 2:
		*((uint32_t *)args[0]) = diskio_svc_handler(args[1],
			(uint32_t *)args[2]);
		break;
	case 3:
		firmware_update(args[0], args[1]);
		break;
	case 4:
		*((uint32_t *)args[0]) = (uint32_t)printf;
	default:
		break;
	}
}

void firmware_update(uint32_t addr, uint32_t count)
{
	if (*((uint32_t *)addr) != NCIdentifier)
		return;

	const uint32_t pageSize = flash_get_page_size(&FLASH_0);
	uint32_t writeCount = pageSize;
	uint32_t destAddr = 0x00080000;
	uint8_t *srcAddr = (uint8_t *)addr;
	while (count > 0) {
		if (count < pageSize)
			writeCount = count;

		flash_unlock(&FLASH_0, destAddr, 1);
		flash_write(&FLASH_0, destAddr, srcAddr, writeCount);
		flash_lock(&FLASH_0, destAddr, 1);

		destAddr += writeCount;
		srcAddr += writeCount;
		count -= writeCount;
	}

	NVIC_SystemReset();
	while (1);
}

int32_t flash_svc_handler(uint32_t *args)
{
	flash_unlock(&FLASH_0, args[0], 1);
	return flash_write(&FLASH_0, args[0], (uint8_t *)args[1], args[2]);
}

#include "fatfs/diskio.c"

uint32_t diskio_svc_handler(int n, uint32_t *diskio_args)
{
	uint32_t ret = 0;

	switch (n) {
	case 0:
		ret = disk_status(diskio_args[0]);
		break;
	case 1:
		ret = disk_initialize(diskio_args[0]);
		break;
	case 2:
		ret = disk_read(diskio_args[0], (BYTE *)diskio_args[1],
			diskio_args[2], diskio_args[3]);
		break;
	case 3:
		ret = disk_write(diskio_args[0], (const BYTE *)diskio_args[1],
			diskio_args[2], diskio_args[3]);
		break;
	case 4:
		ret = disk_ioctl(diskio_args[0], diskio_args[1],
			(void *)diskio_args[2]);
		break;
	default:
		break;
	}

	return ret;
}
