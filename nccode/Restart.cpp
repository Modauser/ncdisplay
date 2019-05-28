/**
 * @file Restart.cpp
 * @brief Fullscreen message asks for machine restart, no other options.
 */
#include "type/Assets.h"
#include "type/Screen.h"

#include <gameduino2/GD2.h>

static const LanguageString restartTitle ({
	"PLEASE RESTART"
});

static const LanguageString restartMessage ({
	"Power button is located\n\non back of unit"
});

static Screen Restart (
	ScreenID::Restart,
	// Parent screen
	ScreenID::Restart,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		GD.ClearColorRGB(TINT_BLUE);
		GD.Clear();

		GD.Begin(BITMAPS);
		GD.ColorRGB(TINT_BLUE);
		GD.Vertex2ii(0, 130, HOMEWTR_HANDLE);
		GD.ColorRGB(WHITE);
		GD.Vertex2ii(90, 200, RESTART_HANDLE);

		GD.cmd_text(136, 90, FONT_LARGE, OPT_CENTER, restartTitle());
		GD.cmd_text(136, 400, FONT_TITLE, OPT_CENTER, restartMessage());
	}
);

