/**
 * @file USBUpdateFiles.cpp
 * @brief Prompt for updating/creating files on the SD card.
 *
 * Files can be placed on a USB drive to have them copied to the SD card, which
 * is handled in USBUpdateFiles().
 */

#include "type/Assets.h"
#include "type/Screen.h"

#include <fatfs/ff.h>
#include <gameduino2/GD2.h>

#include <array>
#include <cstring>

static const std::array<const char *, 35> allowedFiles = {
	"assets.gd3",
	"EngCarb0.txt",
	"EngErr00.txt",
	"English0.txt",
	"EngSet00.txt",
	"EngSetL0.txt",
	"FraCarb0.txt",
	"FreErr00.txt",
	"French00.txt",
	"FreSet00.txt",
	"FreSetL0.txt",
	"GerCarb0.txt",
	"GerErr00.txt",
	"German00.txt",
	"GerSet00.txt",
	"GerSetL0.txt",
	"lock.jpg",
	"nirma14.dat",
	"nirma20.dat",
	"nirmab14.dat",
	"nirmab18.dat",
	"Password.txt",
	"Service0.txt",
	"screensaver1.jpg",
	"screensaver2.jpg",
	"screensaver3.jpg",
	"sleepmode.jpg",
	"SpaCarb0.txt",
	"SpaErr00.txt",
	"Spanish0.txt",
	"SpaSet00.txt",
	"SpaSetL0.txt",
	"startup.jpg",
	"unlock.jpg",
	"usb.jpg"
};

static DIR rootDirectory;
static FILINFO fileInfo;
static char *fileName;
static char srcFile[68];

static bool isAllowedFile(const char *file);
static void failedFileCopy(const char *msg);

/**
 * Renders the copying prompt for the current file.
 * @param extra A function to do extra rendering if necessary
 */
static void renderPrompt(std::function<void(void)> extra = [](){})
{
		GD.ClearColorRGB(NC_BKGND_COLOR);
		GD.ColorRGB(NC_FRGND_COLOR);
		GD.Clear();
		GD.cmd_text(136, 30, 23, OPT_CENTER, "Copying USB Files");
		GD.cmd_text(10, 140, 18, 0, "Copy file?");
		GD.ColorRGB(NC_FDGND_COLOR);
		GD.cmd_text(30, 160, 18, 0, fileName);

		GD.ColorRGB(WHITE);
		GD.Tag(1);
		GD.cmd_button(10, 420, 120, 50, 18, OPT_FLAT, "Copy");
		GD.Tag(2);
		GD.cmd_button(142, 420, 120, 50, 18, OPT_FLAT, "Don't");
		GD.Tag(0);

		extra();

		GD.swap();
}

/**
 * Checks for files on the USB, then prompts to copy them to the SD card if
 * necessary.
 */
void USBUpdateFiles(void)
{
	bool hadFiles = false;

	// Try to load the USB drive
	FRESULT res = f_opendir(&rootDirectory, DRV_USB);
	if (res != FR_OK)
		return;

	while (1) {
		// Get next file
		res = f_readdir(&rootDirectory, &fileInfo);

		// If error or none found, leave the loop
		if (res != FR_OK || fileInfo.fname[0] == '\0')
			break;

		// Skip directorys, and only take files we want
		if ((fileInfo.fattrib & AM_DIR) ||
			fileInfo.fname[0] == '.' ||
			!isAllowedFile(fileInfo.fname))
			continue;

		// Load the file's name and show the prompt
		hadFiles = true;
		fileName = fileInfo.fname;
		renderPrompt();

		// Wait for input
		do {
			delay_ms(10);
			GD.get_inputs();
		} while (GD.inputs.tag == 0);

		// Get tag of pressed button
		int tag = GD.inputs.tag;

		// Wait for release
		do {
			delay_ms(10);
			GD.get_inputs();
		} while (GD.inputs.tag != 0);

		// If 'Copy' was pressed, copy the file
		if (tag == 1) {
			UINT bytesRead;
			FIL src, dst;

			renderPrompt([](){
				GD.ColorRGB(NC_FRGND_COLOR);
				GD.cmd_text(10, 220, 18, 0, "Copying...");
			});

			// Load the USB file
			strcpy(srcFile, DRV_USB);
			strcat(srcFile, fileName);
			res = f_open(&src, srcFile, FA_READ);

			if (res != FR_OK) {
				failedFileCopy("Couldn't open file.");
				continue;
			}

			// Read the entire file into memory
			extern void *UPDATE_LOAD_ADDR;
			FSIZE_t size = f_size(&src);
			res = f_read(&src, UPDATE_LOAD_ADDR, size, &bytesRead);
			if (res != FR_OK || bytesRead != size) {
				failedFileCopy("Couldn't read entire file.");
				continue;
			}

			// Create the file on the SD card (overwrites existing)
			res = f_open(&dst, fileName, FA_WRITE |
				FA_CREATE_ALWAYS);
			if (res != FR_OK) {
				failedFileCopy("Couldn't access SD card.");
				continue;
			}

			// Write the USB file's data to the SD card file
			res = f_write(&dst, UPDATE_LOAD_ADDR, size, &bytesRead);
			if (res != FR_OK || bytesRead != size) {
				failedFileCopy("Partial file copy!");
				continue;
			}

			f_close(&src);
			f_close(&dst);

			renderPrompt([](){
				GD.ColorRGB(NC_FRGND_COLOR);
				GD.cmd_text(10, 220, 18, 0, "Copying...");
				GD.cmd_text(10, 250, 18, 0, "Success.");
			});
			delay_ms(500);
		}
	}

	// Show a finished message if files were prompted for
	if (hadFiles) {
		GD.ClearColorRGB(NC_BKGND_COLOR);
		GD.ColorRGB(NC_FRGND_COLOR);
		GD.Clear();
		GD.cmd_text(136, 30, 23, OPT_CENTER, "Copying USB Files");
		GD.cmd_text(136, 140, 18, OPT_CENTER, "Done.");
		GD.cmd_text(136, 170, 18, OPT_CENTER, "Remove USB after setup complete.");
		GD.swap();
		delay_ms(3000);
	}
}

bool isAllowedFile(const char *file)
{
	for (auto f : allowedFiles) {
		if (strcmp(f, file) == 0)
			return true;
	}

	return false;
}

void failedFileCopy(const char *msg)
{
	renderPrompt([&msg](){
		GD.cmd_text(10, 200, 18, 0, "Copying failed:");
		GD.cmd_text(10, 220, 18, 0, msg);
	});
	delay_ms(3000);
}

