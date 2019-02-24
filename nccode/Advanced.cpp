/**
 * @file Advanced.cpp
 * @brief The advanced settings menu.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

extern const LanguageString *warningMessage;
extern ScreenID warningProceedScreen;
extern void (*warningProceedFunc)(void);

static const LanguageString warningReset ({
	"Resetting to factory defaults will\nclear all saved settings.\n\nContinue?"
});

static const LanguageString warningAutofill ({
	"Internal tanks will be refilled.\nDo not leave unit unattended.\n\nContinue?"
});

static const LanguageString lAdvanced ({
	"Advanced",
	"Fortschritt",
	"Avanc" e_ACUTE,
	"Avanzado"
});

static Screen Advanced (
	ScreenID::Advanced,
	// Parent screen
	ScreenID::Settings,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 90, FONT_TITLE, OPT_CENTER, lAdvanced());
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Settings);
	}),
	Button({0, 120}, Button::drawMenuItem, {
		"USB UPLOAD",
		"USB HOCHLADEN",
		"T" E_ACUTE "L" E_ACUTE "CHARGEMENT USB",
		"CARGAR USB"
	}, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::USBUpload);
	}),
	Button({0, 180}, Button::drawMenuItem, {
		"RESET FACTORY SETTINGS",
		"RESETTEN",
		"R" E_ACUTE "INITIALISER",
		"REINICIAR"
	}, [](bool press) {
		if (!press) {
			warningMessage = &warningReset;
			warningProceedScreen = ScreenID::Restart;
			warningProceedFunc = MainBoard::factoryReset;
			ScreenManager::setCurrent(ScreenID::Warning);
		}
	}),
	Button({0, 240}, Button::drawMenuItem, {
		"SERVICE DETAILS",
		"SERVICE-DETAILS",
		"INFORMATIONS SAV",
		"ASISTENCIA T" E_ACUTE "CNICA"
	}, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::ServiceDetails);
	}),
	Button({0, 300}, Button::drawMenuItem, {
		"SYSTEM OPTIONS",
		"SYSTEMOPTIONEN",
		"OPTIONS DU SYST" E_GRAVE "ME",
		"OPCIONES DEL SISTEMA"
	}, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::SystemOptions);
	}),
	Button({0, 360}, Button::drawMenuItem, {
		"PASSCODE",
		"PASSCODE",
		"CODE D'ACC" E_GRAVE "S",
		"C" O_ACUTE "DIGO DE ACCESO"
	}, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Lock);
	}),
	Button({0, 420}, Button::drawMenuItem, "AUTOFILL", [](bool press) {
		if (!press) {
			warningMessage = &warningAutofill;
			warningProceedFunc = MainBoard::autofill;
			ScreenManager::setCurrent(ScreenID::Warning);
		}
	})
);

