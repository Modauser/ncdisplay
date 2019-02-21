/**
 * @file FillScreen.cpp
 * @brief Screen shown on startup when tanks are being filled.
 */
#include "type/Assets.h"
#include "type/Screen.h"

#include <gameduino2/GD2.h>

const LanguageString *fillMessage = nullptr;
uint32_t fillTint = 0xFFFFFF;

const LanguageString fillCold ({
	"FILLING COLD\n\nWATER SYSTEM"
});

const LanguageString fillHot ({
	"FILLING HOT\n\nWATER SYSTEM"
});

const LanguageString fillSparkling ({
	"FILLING SPARKLING\n\nWATER SYSTEM"
});

static int fillCounter = 0;

static Screen screenFill (
	ScreenID::Fill,
	// Parent screen
	ScreenID::Dispense,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		GD.ClearColorRGB(fillTint);
		GD.Clear();

		GD.Begin(BITMAPS);
		GD.ColorRGB(fillTint);
		GD.Vertex2ii(0, 130, HOMEWTR_HANDLE);
		GD.ColorRGB(WHITE);
		GD.Vertex2ii(83, 174, DROPSTRP_HANDLE, fillCounter);

		if (++fillCounter >= 7)
			fillCounter = 0;

		if (fillMessage != nullptr) {
			GD.cmd_text(136, 340, FONT_LARGE, OPT_CENTER,
				(*fillMessage)(), 30);
		}
	}
);


