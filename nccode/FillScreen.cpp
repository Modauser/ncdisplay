/**
 * @file FillScreen.cpp
 * @brief Screen shown on startup when tanks are being filled.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "Errors.h"
#include "MainBoard.h"

#include <array>
#include <functional>
#include <gameduino2/GD2.h>
#include <tuple>

// For each fill screen (cold, hot, sparkling), there is a tuple/group of
// text, tint color, and function for checking fill status. Screen iterates
// through these
static const std::array<std::tuple<LanguageString, uint32_t,
	std::function<bool(void)>>, 3> fillScreens = {{
	{{
		"FILLING COLD\n\nWATER SYSTEM",
		"F" U_UMLAUT "LLEN\n\nKALTWASSER",
		"REMPLISSAGE DU\n\nSYST" E_GRAVE "ME D'EAU FROIDE",
		"SISTEMA DE LLENADO\n\nDE AGUA FR" I_GRAVE "A"
	}, TINT_BLUE, MainBoard::isColdTankFull},

	{{
		"FILLING HOT\n\nWATER SYSTEM",
		"F" U_UMLAUT "LLEN\n\nHEISSWASSER",
		"REMPLISSAGE DU\n\nSYST" E_GRAVE "ME D'EAU CHAUDE",
		"SISTEMA DE LLENADO\n\nDE AGUA CALIENTE"
	}, TINT_RED, MainBoard::isHotTankFull},

	{{
		"FILLING SPARKLING\n\nWATER SYSTEM",
		"F" U_UMLAUT "LLEN\n\nSODAWASSER",
		"REMPLISSAGE DU\n\nSYST" E_GRAVE "ME D'EAU P" E_ACUTE "TILLANTE",
		"SISTEMA DE LLENADO\n\nDE AGUA CARBONATADA"
	}, TINT_GREEN, MainBoard::isSparklingTankFull}
}};

static auto fillScreen = fillScreens.cbegin();
static unsigned int fillCounter = 0;
static unsigned int checkCounter = 0;

static Screen Fill (
	ScreenID::Fill,
	// Parent screen
	ScreenID::Fill,
	// Initialization function
	[](void) {
		fillCounter = 0;
		checkCounter = 0;

		// See if any tanks are already full
		for (; fillScreen != fillScreens.end() &&
			std::get<2>(*fillScreen)(); fillScreen++)
			delay_ms(500);

		// If all were full, skip this screen
		if (fillScreen == fillScreens.end())
			ScreenManager::setCurrent(ScreenID::SetupComplete);
	},
	// Pre-draw function
	[](void) {
		const auto& tint = std::get<1>(*fillScreen);
		GD.ClearColorRGB(tint);
		GD.Clear();

		// Draw background and drop animation
		GD.Begin(BITMAPS);
		GD.ColorRGB(tint);
		GD.Vertex2ii(0, 130, HOMEWTR_HANDLE);
		GD.ColorRGB(WHITE);
		GD.Vertex2ii(106, 203, DROPSTRP_HANDLE, fillCounter);

		if (++fillCounter >= 7)
			fillCounter = 0;

		// Put fill text
		GD.cmd_text(136, 330, FONT_LARGE, OPT_CENTER,
			std::get<0>(*fillScreen)(), 30);

		// Check tank status every second
		if (++checkCounter >= 4) {
			checkCounter = 0;

			while (std::get<2>(*fillScreen)()) {
				// Tank full, go to next
				fillScreen++;

				// If all are full, continue to setup complete
				if (fillScreen == fillScreens.end()) {
					ScreenManager::setCurrent(
						ScreenID::SetupComplete);
					break;
				}
			}

			// Check for error
			if (Error::check())
				Error::showStartup();
		} 

		delay_ms(200);
	}
);

