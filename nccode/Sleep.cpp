#include <gameduino2/GD2.h>

#include "Assets.h"
#include "Screens.h"

static char sleepImagePath[] = "sleep0.jpg";
static int sleepImageCurrent = 1;
static int sleepImageLast = -1;
static unsigned int sleepImageCounter;

Screen screenSleep (
	// Parent screen
	&screenDispense,
	// Buttons
	nullptr, 0,
	// Initialization function
	[](void) {
		sleepImageCounter = 0;
	},
	// Pre-draw function
	[](void) {
		if (sleepImageCurrent != sleepImageLast) {
			sleepImageLast = sleepImageCurrent;

			sleepImagePath[5] = sleepImageCurrent + '0';
			GD.BitmapHandle(SLEEP_HANDLE);
			GD.cmd_loadimage(0, 0);
			GD.load(sleepImagePath);
			GD.finish();
		}

		// Draw the sleep image
		GD.Clear();
		GD.ColorRGB(WHITE);
		GD.Begin(BITMAPS);
		GD.Vertex2ii(0, 0, SLEEP_HANDLE);

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.Begin(RECTS);
		GD.Vertex2ii(0, 420);
		GD.Vertex2ii(272, 480);

		GD.ColorRGB(WHITE);
		GD.cmd_text(136, 450, FONT_SMALL, OPT_CENTER, LanguageString({
			"TOUCH TO BEGIN"
		})());

		sleepImageCounter += 10;
		if (sleepImageCounter >= SLEEP_IMGSWITCH) {
			sleepImageCounter = 0;

			sleepImageCurrent++;
			if (sleepImageCurrent > 3)
				sleepImageCurrent = 1;
		}

		GD.get_inputs();
		if (GD.inputs.x != -32768 && GD.inputs.y != -32768) {
			// Wait for release
			while (GD.inputs.x != -32768 && GD.inputs.y != -32768) {
				GD.get_inputs();
				delay_ms(10);
			}

			// Wake up, go to main dispense screen
			screenCurrent = &screenDispense;
		}
	}
);

