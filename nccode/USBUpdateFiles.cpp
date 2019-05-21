/**
 * @file USBUpdateFiles.cpp
 * @brief Screen for updating config files from the USB drive.
 */
#include "type/Assets.h"
#include "type/Screen.h"

#include <fatfs/ff.h>
#include <gameduino2/GD2.h>

#include <cstring>

static DIR rootDirectory;
static FILINFO fileInfo;
static char *fileName;
static char srcFile[68];

static void failedFileCopy(const char *msg);

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

void USBUpdateFiles(void)
{
	bool hadFiles = false;
	FRESULT res = f_opendir(&rootDirectory, DRV_USB);
	if (res != FR_OK)
		return;

	while (1) {
		res = f_readdir(&rootDirectory, &fileInfo);
		if (res != FR_OK || fileInfo.fname[0] == '\0')
			break;
		if (fileInfo.fname[0] == '.' || (fileInfo.fattrib & AM_DIR))
			continue;

		hadFiles = true;
		fileName = fileInfo.fname;

		renderPrompt();

		do {
			delay_ms(10);
			GD.get_inputs();
		} while (GD.inputs.tag == 0);

		int tag = GD.inputs.tag;

		do {
			delay_ms(10);
			GD.get_inputs();
		} while (GD.inputs.tag != 0);

		if (tag == 1) {
			UINT bytesRead;
			FIL src, dst;

			renderPrompt([](){
				GD.ColorRGB(NC_FRGND_COLOR);
				GD.cmd_text(10, 220, 18, 0, "Copying...");
			});

			strcpy(srcFile, DRV_USB);
			strcat(srcFile, fileName);
			res = f_open(&src, srcFile, FA_READ);

			if (res != FR_OK) {
				failedFileCopy("Couldn't open file.");
				continue;
			}

			extern void *UPDATE_LOAD_ADDR;
			FSIZE_t size = f_size(&src);
			res = f_read(&src, UPDATE_LOAD_ADDR, size, &bytesRead);
			if (res != FR_OK || bytesRead != size) {
				failedFileCopy("Couldn't read entire file.");
				continue;
			}

			res = f_open(&dst, fileName, FA_WRITE |
				FA_CREATE_ALWAYS);
			if (res != FR_OK) {
				failedFileCopy("Couldn't access SD card.");
				continue;
			}

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

void failedFileCopy(const char *msg)
{
	renderPrompt([&msg](){
		GD.cmd_text(10, 200, 18, 0, "Copying failed:");
		GD.cmd_text(10, 220, 18, 0, msg);
	});
	delay_ms(3000);
}

