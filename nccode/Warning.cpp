#include <gameduino2/GD2.h>

#include "Assets.h"
#include "Screens.h"

static Button buttonsWarning[] = {
	Button(1, {0, 360}, Button::drawFullWidth, "YES", [](bool pressed) {
		if (!pressed)
			screenCurrent = &screenAdvanced;
	}),
	Button(2, {0, 420}, Button::drawFullWidth, "CANCEL", [](bool pressed) {
		if (!pressed)
			screenCurrent = &screenAdvanced;
	})
};

const LanguageString *warningMessage = nullptr;

Screen screenWarning (
	// Parent screen
	&screenAdvanced,
	// Buttons
	buttonsWarning, 2,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		GD.ClearColorRGB(0xC00000);
		GD.Clear();


		GD.Begin(BITMAPS);
		GD.ColorRGB(0xDD0000);
		GD.Vertex2ii(0, 30, HOMEWTR_HANDLE);

		GD.ColorRGB(0xFF0000);
		GD.Begin(RECTS);
		GD.Vertex2ii(30, 120);
		GD.Vertex2ii(242, 150);
		GD.ColorRGB(WHITE);
		GD.Vertex2ii(31, 121);
		GD.Vertex2ii(241, 149);

		if (warningMessage != nullptr) {
			GD.cmd_text(136, 180, FONT_LIGHT, OPT_CENTERX,
				(*warningMessage)(), 30);
		}

		GD.ColorRGB(0xFF0000);
		GD.cmd_text(136, 135, FONT_SMALL, OPT_CENTER, "WARNING");
	}
);


