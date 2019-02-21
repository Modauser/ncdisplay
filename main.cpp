#include <atmel_start.h>
#include <gameduino2/GD2.h>
#include <config/hpl_sercom_config.h>
#include <spi_lite.h>

// Enables USB drive detection and access
#define ALLOW_USB

//extern void setup(void);
//extern void loop(void);

constexpr uint32_t NCIdentifier = 0x31415926;
static uint32_t *ncident = (uint32_t *)0x00080000;
static void (*ncmain)(void) = (void (*)(void))(0x00080004 | 1);

static FATFS sdmmc_fatfs;
static FATFS msc_fatfs;
int initDisks(void);

#include "fatfs/diskio.c"

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

	// Needed so USB interrupts can happen within fatfs service call
	NVIC_SetPriority(SVCall_IRQn, 250);

	// Enable SPI
	SPI_0_enable();

	initDisks();

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

uint32_t fatfs_svc_handler(uint32_t *fatfs_args);
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
		*((uint32_t *)args[0]) = fatfs_svc_handler((uint32_t *)args[1]);
		break;
	case 3:
		firmware_update(args[0], args[1]);
		break;
	case 4:
		*((uint32_t *)args[0]) = (uint32_t)printf;
		break;
	case 5:
		*((uint32_t *)args[0]) = getchar();
		break;
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

uint32_t fatfs_svc_handler(uint32_t *fatfs_args)
{
	uint32_t ret = 0;

	switch (fatfs_args[0]) {
	case 0:
		ret = f_open((FIL *)fatfs_args[1], (const TCHAR *)fatfs_args[2],
			fatfs_args[3]);
		break;
	case 1:
		ret = f_close((FIL *)fatfs_args[1]);
		break;
	case 2:
		ret = f_read((FIL *)fatfs_args[1], (void *)fatfs_args[2],
			fatfs_args[3], (UINT *)fatfs_args[4]);
		break;
	case 3:
		ret = f_write((FIL *)fatfs_args[1], (const void *)fatfs_args[2],
			fatfs_args[3], (UINT *)fatfs_args[4]);
		break;
	case 4:
		ret = f_unlink((const TCHAR *)fatfs_args[1]);
		break;
	case 5:
		ret = (uint32_t)f_gets((TCHAR *)fatfs_args[1], fatfs_args[2],
			(FIL *)fatfs_args[3]);
		break;
	default:
		break;
	}

	return ret;
}

int initDisks(void)
{
	dstatus_t status;
	FRESULT result;
	int ret = 0;

	// Begin searching for a connected SD card
	status = STA_NOINIT;
	do {
		status = disk_status(DEV_SD);
		delay_ms(200);
	} while (status != 0 && status != STA_NODISK);

	if (status == STA_NODISK) {
		//printf("SD card not found.\r\n");
		ret = -1;
		return ret;
	} else {
		// SD card found; mount it
		result = f_mount(&sdmmc_fatfs, DRV_SD, 1);
		//if (result != FR_OK)
			//printf("SD card mounting failed (%d).\r\n", result);
	}

#ifdef ALLOW_USB
	// Search for a USB driver with a 3 second timeout
	status = STA_NOINIT;
	usbhc_start(&USB_HOST_CORE_INSTANCE_inst);
	int tries = 15;
	do {
		status = disk_status(DEV_USB);
		delay_ms(200);
	} while (status != 0 && status != STA_NODISK && (--tries > 0));

	if (tries <= 0)
		status = STA_NODISK;

	if (status == STA_NODISK) {
		//printf("USB drive not found.\r\n");
		ret = 1;
	} else {
		// USB drive found; mount it
		result = f_mount(&msc_fatfs, DRV_USB, 1);
		//if (result != FR_OK)
		//	printf("USB drive mounting failed (%d).\r\n", result);
	}
#endif // ALLOW_USB

	(void)result;
	return ret;
}
