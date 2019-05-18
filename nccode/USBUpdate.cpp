/**
 * @file USBUpdate.cpp
 * @brief Code to check for and then apply potential firmware updates.
 */
#include "type/Assets.h"

#include <gameduino2/GD2.h>
#include <fatfs/ff.h>
#include <stdint.h>
#include <string.h>

// Should RAM space become an issue, some form of streamable decompression will
// be implemented. The current candidate is QuickLZ, which allows for streamed
// decompression; this could save ~50KB of memory.

// Place the update immediately after RAM space reserved for stack/variables.
extern unsigned char _estack;
void *UPDATE_LOAD_ADDR = (void *)((uint32_t)((char *)&_estack + 15) & ~(15));
static const uint32_t UPDATE_MAX_SIZE = 0x20030000 - (uint32_t)UPDATE_LOAD_ADDR;

constexpr const char *USBUpdatePath = DRV_USB "update.bin";

extern void firmwareUpdate(uint32_t addr, uint32_t count);

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
	GD.cmd_text(0, 0, 18, 0, "Update found. Loading...");
	GD.swap();

	// Load file into unused memory
	// Copy data
	result = f_read(&fd, UPDATE_LOAD_ADDR, size, &bytesRead);
	if (result != FR_OK || bytesRead != size)
		return false;

	GD.Clear();
	GD.cmd_text(0, 0, 18, 0, "Update found. Loading...");
	GD.cmd_text(0, 20, 18, 0, "Ready. Updating...");
	GD.cmd_text(0, 40, 18, 0, "Will reset when finished.");
	GD.swap();

	f_close(&fd);
	f_unlink(USBUpdatePath);

	firmwareUpdate((uint32_t)UPDATE_LOAD_ADDR, size);

	return true;
}

