/**
 * @file USBUpdate.cpp
 * @brief Code to check for and then apply potential firmware updates.
 */

#include "type/Assets.h"
#include "SystemCalls.h"

#include <gameduino2/GD2.h>
#include <fatfs/ff.h>
#include <stdint.h>
#include <string.h>

// Should RAM space become an issue, streamable decompression may be
// implemented. QuickLZ may be useful for this, and could save ~50KB of memory.

/**
 * Linker variable who's address is after all program data in memory.
 */
extern unsigned char _estack;

/**
 * Address to load updates to (i.e. immediately after all program data, to
 * maximize usable space).
 */
void *UPDATE_LOAD_ADDR = (void *)((uint32_t)((char *)&_estack + 15) & ~(15));

/**
 * The maximum possible update size.
 */
static const uint32_t UPDATE_MAX_SIZE = 0x20030000 - (uint32_t)UPDATE_LOAD_ADDR;

/**
 * The path that a firmware update is expected to be found at.
 */
constexpr const char *USBUpdatePath = DRV_USB "update.bin";

/**
 * Checks for a firmware update, applying it if found.
 * If an update is found, this function will not return; instead, the display
 * will reset upon update completion.
 */
bool USBUpdateCheck(void)
{
	UINT bytesRead;

	// Try to load the update
	FIL fd;
	FRESULT result = f_open(&fd, USBUpdatePath, FA_READ);
	if (result != FR_OK)
		return false;

	// Update found: get it's size
	FSIZE_t size = f_size(&fd);
	if (size > UPDATE_MAX_SIZE)
		return true;

	GD.ClearColorRGB(NC_BKGND_COLOR);
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Clear();
	GD.cmd_text(136, 30, 23, OPT_CENTER, "USB Firmware Update");
	GD.cmd_text(10, 140, 18, 0, "Loading update...");
	GD.swap();

	// Load the update into memory
	result = f_read(&fd, UPDATE_LOAD_ADDR, size, &bytesRead);
	if (result != FR_OK || bytesRead != size)
		return true;

	GD.ClearColorRGB(NC_BKGND_COLOR);
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Clear();
	GD.cmd_text(136, 30, 23, OPT_CENTER, "USB Firmware Update");
	GD.cmd_text(10, 140, 18, 0, "Loading update...");
	GD.cmd_text(10, 170, 18, 0, "Update loaded.");
	GD.cmd_text(10, 200, 18, 0, "Applying update...");
	GD.swap();

	// Close the update file and delete it from the USB drive
	f_close(&fd);
	f_unlink(USBUpdatePath);

	// Begin the firmware update
	firmwareUpdate((uint32_t)UPDATE_LOAD_ADDR, size, &GD);

	// Display will reset; this will not be reached
	return true;
}

