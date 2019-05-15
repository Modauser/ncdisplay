/**
 * @file USBUpload.cpp
 * @brief Screen explaining USB firmware updating.
 */
#include "type/Assets.h"
#include "type/Screen.h"

#include <gameduino2/GD2.h>

static Screen USBUpload (
	ScreenID::USBUpload,
	// Parent screen
	ScreenID::Advanced,
	// Initialization function
	[](void) {
		loadImage(FREE_HANDLE, "usb.jpg");
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.Begin(BITMAPS);
		GD.Vertex2ii(112, 176, FREE_HANDLE);

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
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::Advanced);
	})
);

