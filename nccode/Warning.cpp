/**
 * @file Warning.cpp
 * @brief Customizable screen for warning messages (factory reset/autofill).
 */

#include "type/Assets.h"
#include "type/Screen.h"

#include <gameduino2/GD2.h>

// These variables are set in Advanced.cpp before this screen is switched to
// Warning message
const LanguageString *warningMessage = nullptr;
// Text for the yes/accept button
const LanguageString *warningYes = nullptr;
// Screen to load if yes/accept is pressed
ScreenID warningProceedScreen = ScreenID::Advanced;
// Function to call if yes/accept is pressed (called before screen switch)
void (*warningProceedFunc)(void) = nullptr;

// Sets the yes/accept button text to what is currently given
static void setYesText(void);

static Screen Warning (
	ScreenID::Warning,
	// Parent screen
	ScreenID::Advanced,
	// Initialization function
	[](void) {
		setYesText();
	},
	// Pre-draw function
	[](void) {
		GD.ClearColorRGB(TINT_RED);
		GD.Clear();

		GD.Begin(BITMAPS);
		GD.ColorRGB(TINT_RED);
		GD.Vertex2ii(0, 40, HOMEWTR_HANDLE);

		// Warning title box
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

		// Text inside warning title box
		// Placed after warning message's cmd_text to save a ColorRGB
		// call
		GD.ColorRGB(0xFF0000);
		GD.cmd_text(136, 165, FONT_SMALL, OPT_CENTER, LanguageString({
			"WARNING",
			"VORSICHT",
			"ATTENTION",
			"ADVERTENCIA"
		})());
	},
	// Buttons
	Button({0xF, 360}, Button::drawRedFullWidth, "YES",
	[](bool pressed) {
		if (!pressed) {
			if (warningProceedFunc != nullptr)
				warningProceedFunc();
			ScreenManager::setCurrent(warningProceedScreen);
		}
	}),
	Button({0xC, 420}, Button::drawRedFullWidth, {
		"CANCEL",
		"CANCEL",
		"ANNULER",
		"CANCELAR"
	}, [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::Advanced);
	})
);

void setYesText(void)
{
	Warning.getButton(0).setText(*warningYes);
}

