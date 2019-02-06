#include <gameduino2/GD2.h>

#include "Assets.h"
#include "Screens.h"

extern const LanguageString *warningMessage;

static const LanguageString warningReset ({
	"Resetting to factory defaults will\nclear all saved settings.\n\nContinue?"
});

static const LanguageString warningAutofill ({
	"Internal tanks will be refilled.\nDo not leave unit unattended.\n\nContinue?"
});

static Button buttonsAdvanced[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			screenCurrent = &screenSettings;
	}),
	Button(2, {0, 122}, Button::drawMenuItem, {
		"USB UPLOAD",
		"USB HOCHLADEN",
		"T" E_ACUTE "L" E_ACUTE "CHARGEMENT USB",
		"CARGAR USB"
	}, [](bool press) {
		if (!press)
			screenCurrent = &screenUSBUpload;
	}),
	Button(3, {0, 182}, Button::drawMenuItem, {
		"RESET FACTORY SETTINGS",
		"RESETTEN",
		"R" E_ACUTE "INITIALISER",
		"REINICIAR"
	}, [](bool press) {
		if (!press) {
			warningMessage = &warningReset;
			screenCurrent = &screenWarning;
		}
	}),
	Button(4, {0, 242}, Button::drawMenuItem, {
		"SERVICE DETAILS",
		"SERVICE-DETAILS",
		"INFORMATIONS SAV",
		"ASISTENCIA T" E_ACUTE "CNICA"
	}, [](bool press) {
		if (!press)
			screenCurrent = &screenServiceDetails;
	}),
	Button(5, {0, 302}, Button::drawMenuItem, {
		"SYSTEM OPTIONS",
		"SYSTEMOPTIONEN",
		"OPTIONS DU SYST" E_GRAVE "ME",
		"OPCIONES DEL SISTEMA"
	}, [](bool press) {
		if (!press)
			screenCurrent = &screenSystemOptions;
	}),
	Button(6, {0, 362}, Button::drawMenuItem, {
		"PASSCODE",
		"PASSCODE",
		"CODE D'ACC" E_GRAVE "S",
		"C" O_ACUTE "DIGO DE ACCESO"
	}, [](bool press) {
		if (!press)
			screenCurrent = &screenLock;
	}),
	Button(7, {0, 422}, Button::drawMenuItem, "AUTOFILL", [](bool press) {
		if (!press) {
			warningMessage = &warningAutofill;
			screenCurrent = &screenWarning;
		}
	}),
};

static const LanguageString aAdvanced ({
	"Advanced",
	"Fortschritt",
	"Avanc" e_ACUTE,
	"Avanzado"
});

Screen screenAdvanced (
	// Parent screen
	&screenSettings,
	// Buttons
	buttonsAdvanced, 7,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 70, FONT_TITLE, OPT_CENTERX, aAdvanced());
	}
);

