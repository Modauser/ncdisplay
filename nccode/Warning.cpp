/**
 * @file Warning.cpp
 * @brief Customizable screen for warning messages (factory reset/autofill).
 */

#include "type/Assets.h"
#include "type/Screen.h"

#include <gameduino2/GD2.h>

/**
 * The warning message to be displayed.
 * If left as nullptr, no message is shown.
 */
const LanguageString *warningMessage = nullptr;
ScreenID warningProceedScreen = ScreenID::Advanced;
void (*warningProceedFunc)(void) = nullptr;

static Screen Warning (
	ScreenID::Warning,
	// Parent screen
	ScreenID::Advanced,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		GD.ClearColorRGB(TINT_RED);
		GD.Clear();

		GD.Begin(BITMAPS);
		GD.ColorRGB(TINT_RED);
		GD.Vertex2ii(0, 40, HOMEWTR_HANDLE);

		GD.ColorRGB(0xFF0000);
		GD.Begin(RECTS);
		GD.Vertex2ii(30, 150);
		GD.Vertex2ii(242, 180);
		GD.ColorRGB(WHITE);
		GD.Vertex2ii(31, 151);
		GD.Vertex2ii(241, 179);

		if (warningMessage != nullptr) {
			GD.cmd_text(136, 210, FONT_LIGHT, OPT_CENTERX,
				(*warningMessage)(), 30);
		}

		GD.ColorRGB(0xFF0000);
		GD.cmd_text(136, 165, FONT_SMALL, OPT_CENTER, "WARNING");
	},
	// Buttons
	Button({0xF, 360}, Button::drawRedFullWidth, "YES", [](bool pressed) {
		if (!pressed) {
			if (warningProceedFunc != nullptr)
				warningProceedFunc();
			ScreenManager::setCurrent(warningProceedScreen);
		}
	}),
	Button({0xC, 420}, Button::drawRedFullWidth, "CANCEL", [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::Advanced);
	})
);

