#include <gameduino2/GD2.h>
#include <fatfs/ff.h>
#include <stdint.h>
#include <string.h>

#include "Assets.h"
#include "driver_init.h"

// Should RAM space become an issue, some form of streamable decompression will
// be implemented. The current candidate is QuickLZ, which allows for streamed
// decompression; this could save ~50KB of memory.

// Place the update immediately after RAM space reserved for stack/variables.
extern unsigned char _estack;
static void *UPDATE_LOAD_ADDR = (void *)((uint32_t)((char *)&_estack + 15) & ~(15));
static const uint32_t UPDATE_MAX_SIZE = 0x20030000 - (uint32_t)UPDATE_LOAD_ADDR;

constexpr const char *USBUpdatePath = DRV_SD "update.bin";

void USBUpdateWriteToFlash(uint32_t size);

bool USBUpdateCheck(void)
{
	UINT bytesRead;

	FIL fd;
	FRESULT result = f_open(&fd, USBUpdatePath, FA_READ);

	if (result != FR_OK)
		return false;

	// Get file size
	FSIZE_t size = f_size(&fd);
	if (size > UPDATE_MAX_SIZE)
		return false;

	GD.ClearColorRGB(NC_BKGND_COLOR);
	GD.Clear();
	GD.cmd_text(0, 0, FONT_MESG, 0, "Update found. Loading...");
	GD.swap();

	// Load file into unused memory
	// Copy data
	result = f_read(&fd, UPDATE_LOAD_ADDR, size, &bytesRead);
	if (result != FR_OK || bytesRead != size)
		return false;

	GD.cmd_text(0, 20, FONT_MESG, 0, "Ready. Updating...");
	GD.cmd_text(0, 40, FONT_MESG, 0, "Will reset when finished.");
	GD.swap();

	USBUpdateWriteToFlash(size);

	f_close(&fd);
	return true;
}

__attribute__ ((section(".usbupdate")))
void USBUpdateWriteToFlash(uint32_t size)
{
	uint32_t pageSize = flash_get_page_size(&FLASH_0);
	uint8_t *src = (uint8_t *)UPDATE_LOAD_ADDR;
	for (uint32_t written = 0x1000; written < size; written += pageSize)
		flash_write(&FLASH_0, written, src + written, pageSize);

	// We can't really return... so reset
	__NVIC_SystemReset();
}

