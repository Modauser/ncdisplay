#include <gameduino2/GD2.h>

#include "Assets.h"
#include "Screens.h"

static Button buttonsUSBUpload[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			screenCurrent = &screenAdvanced;
	})
};

Screen screenUSBUpload (
	// Parent screen
	&screenAdvanced,
	// Buttons
	buttonsUSBUpload, 1,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 150, FONT_TITLE, OPT_CENTERX, LanguageString({
			"1. Connect USB",
			"1. USB Verbinden",
			"1. Connecter la cl" e_ACUTE " USB",
			"1. Conectar USB"
		})());
		GD.cmd_text(136, 300, FONT_TITLE, OPT_CENTERX, LanguageString({
			"2. Restart ION to upload",
			"2. Neustart ION zum Hochladen",
			"2. Red" e_ACUTE "marrer ION pour t" e_ACUTE "l" e_ACUTE
			"charger",
			"2. Rencender el ION para subir la informaci" o_ACUTE
			"n"
		})());
	}
);

