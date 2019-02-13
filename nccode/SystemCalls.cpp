#include <fatfs/ff.h>
#include <diskio.h>
#include <stdint.h>

// Enables USB drive detection and access
//#define ALLOW_USB

static FATFS sdmmc_fatfs;
static FATFS msc_fatfs;

uint32_t getPrintf(void)
{
	uint32_t addr = 0;
	asm("\
		mov r0, %0; \
		svc 4; \
	" :: "r" (&addr));
	return addr;
}

void firmwareUpdate(uint32_t addr, uint32_t count)
{
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 3; \
	" :: "r" (addr), "r" (count));
}

uint8_t spi_xfer_byte(uint8_t send)
{
	uint8_t recv = 0;
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 1; \
	" :: "r" (&recv), "r" (send));
	return recv;
}

extern "C" {

void delay_ms(uint32_t ms)
{
	asm("\
		mov r1, %0; \
		svc 0; \
	" :: "r" (ms));
}

dstatus_t disk_status(BYTE pdrv)
{
	dstatus_t result = 0;
	uint32_t _pdrv = pdrv;
	asm("\
		mov r0, %0; \
		mov r1, 0; \
		mov r2, %1; \
		svc 2; \
	" :: "r" (&result), "r" (&_pdrv));
	return result;
}

dstatus_t disk_initialize(BYTE pdrv)
{
	dstatus_t result = 0;
	uint32_t _pdrv = pdrv;
	asm("\
		mov r0, %0; \
		mov r1, 1; \
		mov r2, %1; \
		svc 2; \
	" :: "r" (&result), "r" (&_pdrv));
	return result;
}

dresult_t disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
	dresult_t result = RES_OK;
	uint32_t args[4] = { pdrv, (uint32_t)buff, sector, count };
	asm("\
		mov r0, %0; \
		mov r1, 2; \
		mov r2, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

dresult_t disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
	dresult_t result = RES_OK;
	uint32_t args[4] = { pdrv, (uint32_t)buff, sector, count };
	asm("\
		mov r0, %0; \
		mov r1, 3; \
		mov r2, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

dresult_t disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
	dresult_t result = RES_OK;
	uint32_t args[3] = { pdrv, cmd, (uint32_t)buff };
	asm("\
		mov r0, %0; \
		mov r1, 4; \
		mov r2, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

} // extern "C"


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
	// Search for a USB driver with a 5 second timeout
	status = STA_NOINIT;
	usbhc_start(&USB_HOST_CORE_INSTANCE_inst);
	int tries = 25;
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

	return ret;
}
