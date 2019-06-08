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

		for (; fillScreen != fillScreens.end() &&
			std::get<2>(*fillScreen)(); fillScreen++)
			delay_ms(500);

		if (fillScreen == fillScreens.end())
			ScreenManager::setCurrent(ScreenID::SetupComplete);

	},
	// Pre-draw function
	[](void) {
		const auto& tint = std::get<1>(*fillScreen);
		GD.ClearColorRGB(tint);
		GD.Clear();

		GD.Begin(BITMAPS);
		GD.ColorRGB(tint);
		GD.Vertex2ii(0, 130, HOMEWTR_HANDLE);
		GD.ColorRGB(WHITE);
		GD.Vertex2ii(106, 203, DROPSTRP_HANDLE, fillCounter);

		if (++fillCounter >= 7)
			fillCounter = 0;

		GD.cmd_text(136, 330, FONT_LARGE, OPT_CENTER,
			std::get<0>(*fillScreen)(), 30);

		if (++checkCounter == 4) {
			checkCounter = 0;
			if (std::get<2>(*fillScreen)()) {
				fillScreen++;
				if (std::get<1>(*fillScreen) == 0xFF0000 &&
					!MainBoard::canDispenseHot())
					fillScreen++;
				if (std::get<1>(*fillScreen) == 0x00FF00 &&
					!MainBoard::canDispenseSparkling())
					fillScreen++;
				if (fillScreen == fillScreens.end())
					ScreenManager::setCurrent(ScreenID::SetupComplete);
			}

			if (Error::check())
				Error::showStartup();
		}

		delay_ms(200);
	}
);


