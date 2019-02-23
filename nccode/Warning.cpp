/**
 * @file Warning.cpp
 * @brief Customizable screen for warning messages (factory reset/autofill).
 */
#include "type/Assets.h"
#include "type/Screen.h"

#include <gameduino2/GD2.h>

const LanguageString *warningMessage = nullptr;
ScreenID warningProceedScreen = ScreenID::Advanced;

static Screen Warning (
	ScreenID::Warning,
	// Parent screen
	ScreenID::Advanced,
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
	},
	// Buttons
	Button({0, 360}, Button::drawRedFullWidth, "YES", [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(warningProceedScreen);
	}),
	Button({0, 420}, Button::drawRedFullWidth, "CANCEL", [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::Advanced);
	})
);


