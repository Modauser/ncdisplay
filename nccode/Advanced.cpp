/**
 * @file Advanced.cpp
 * @brief The advanced settings menu.
 */

#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

// External variables for the warning screens. We set them if we plan to show
// a warning screen.
extern const LanguageString *warningMessage;
extern const LanguageString *warningYes;
extern ScreenID warningProceedScreen;
extern void (*warningProceedFunc)(void);

static const LanguageString warningReset ({
	"Resetting to factory defaults will\nclear all saved settings.\n\nContinue?",
	"Auf Fabrikseinstellungen resetten\nwird Alle gespeicherten\n"
		"Einstellungen werden geloscht.\n\nWeiter?",
	"R" e_ACUTE "initialiser aux d" e_ACUTE "fauts d'usine effacera\n"
		"toutes les donn" e_ACUTE "es sauvegard" e_ACUTE "es.\n\n"
		"Continuer?",
	"Al restablecer la\nconfiguraci" o_ACUTE "n de f" a_ACUTE "brica\n"
		"se borrar" a_ACUTE "n todos los\najustes guardados\nContinuar?"
});

static const LanguageString warningAutofill ({
	"Internal tanks will be refilled.\nDo not leave unit unattended.\n\nContinue?"
});

static const LanguageString warningResetYes ({
	"RESET",
	"RESETTEN",
	"R" E_ACUTE "INITIALISER",
	"REINICAR"
});
static const LanguageString warningAutofillYes = "YES";

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
			warningYes = &warningResetYes;
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
			warningYes = &warningAutofillYes;
			warningProceedFunc = MainBoard::autofill;
			ScreenManager::setCurrent(ScreenID::Warning);
		}
	})
);

