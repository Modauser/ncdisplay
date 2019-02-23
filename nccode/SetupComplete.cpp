/**
 * @file SetupComplete.cpp
 * @brief The "setup complete" screen shown before the dispense screen.
 */
#include "type/Assets.h"
#include "type/Screen.h"

#include <gameduino2/GD2.h>

static unsigned int holdCounter = 0;

static const LanguageString setupComplete ({
	"SET UP COMPLETE",
});
static const LanguageString allowHours ({
	"Allow 1-2 Hours for\n\nIdeal Temperature",
});

static Screen SetupComplete (
	// Our ID
	ScreenID::SetupComplete,
	// Parent screen
	ScreenID::Dispense,
	// Initialization function
	[](void) {
		holdCounter = 0;
	},
	// Pre-draw function
	[](void) {
		GD.ClearColorRGB(NC_BKGND_COLOR);
		GD.Clear();

		GD.Begin(BITMAPS);
		GD.Vertex2ii(0, 130, HOMEWTR_HANDLE);

		GD.ColorRGB(NC_FDGND_COLOR);
		GD.cmd_text(136, 80, FONT_LARGE, OPT_CENTER, setupComplete());
		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 400, FONT_TITLE, OPT_CENTER, allowHours());

		// 5 second timeout to dispense screen
		if (++holdCounter >= 500)
			ScreenManager::setCurrent(ScreenID::Dispense);
	}
);

