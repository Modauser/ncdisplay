#include <gameduino2/GD2.h>

#include "Assets.h"
#include "Settings.h"
#include "Screens.h"

static bool sleepModeOn = false;
static bool sleepSettingHours = false;
static bool sleepSettingWeekday = false;

static unsigned int sleepHours[4] = {
	0, 0, 0, 0
};

static AMPMFormat sleepHourString;

void sleepSetVisibilities(void);

static Button buttonsSleepMode[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press) {
			if (sleepSettingHours) {
				if (!sleepSettingWeekday)
					sleepSettingWeekday = true;
				else
					sleepSettingHours = false;

				sleepSetVisibilities();
			} else {
				screenCurrent = &screenSettings;
			}
		}
	}),
	Button(2, {180, 80}, Button::drawToggle, [](bool press) {
		if (!press) {
			sleepModeOn ^= true;

			buttonsSleepMode[1].setForcePressed(sleepModeOn);
			buttonsSleepMode[2].setVisibility(sleepModeOn);
		}
	}),
	Button(3, {0, 120}, Button::drawMenuItem, {
		"CUSTOMIZE HOURS",
		"ANPASSEN",
		"PERSONNALISER",
		"PERSONALIZAR"
	},
	[](bool press) {
		if (!press) {
			sleepSettingHours = true;
			sleepSettingWeekday = true;

			sleepSetVisibilities();
		}
	}),
	Button(4, {0, 422}, Button::drawFullWidth, lStringSave, [](bool press) {
		if (press)
			return;

		if (sleepSettingHours & sleepSettingWeekday) {
			sleepSettingWeekday = false;
			buttonsSleepMode[3].setText(lStringSave);
		} else {
			screenCurrent = &screenSettings;
		}
	}),
	Button(5, {34, 210}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		auto& hour = sleepHours[sleepSettingWeekday ? 0 : 2];
		if (hour < 23)
			hour++;
		else
			hour = 0;
	}),
	Button(6, {34, 282}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		auto& hour = sleepHours[sleepSettingWeekday ? 0 : 2];
		if (hour > 0)
			hour--;
		else
			hour = 23;
	}),
	Button(7, {168, 210}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		auto& hour = sleepHours[sleepSettingWeekday ? 1 : 3];
		if (hour < 23)
			hour++;
		else
			hour = 0;
	}),
	Button(8, {168, 282}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		auto& hour = sleepHours[sleepSettingWeekday ? 1 : 3];
		if (hour > 0)
			hour--;
		else
			hour = 23;
	})
};

void sleepSetVisibilities(void)
{
	buttonsSleepMode[1].setVisibility(!sleepSettingHours);
	buttonsSleepMode[2].setVisibility(sleepModeOn & !sleepSettingHours);
	buttonsSleepMode[4].setVisibility(sleepSettingHours);
	buttonsSleepMode[5].setVisibility(sleepSettingHours);
	buttonsSleepMode[6].setVisibility(sleepSettingHours);
	buttonsSleepMode[7].setVisibility(sleepSettingHours);

	buttonsSleepMode[3].setText((sleepSettingHours & sleepSettingWeekday) ?
		lStringNext : lStringSave);
}

static const LanguageString smWeekdays ({
	"Monday - Friday:",
	"Montag - Freitag:",
	"Lundi - Vendredi:",
	"Lunes - Viernes:"
});
static const LanguageString smWeekend ({
	"Saturday - Sunday:",
	"Samstag - Sonntag:",
	"Samedi - Dimanche:",
	"Sabado - Domingo:"
});

Screen screenSleepMode (
	// Parent screen
	&screenSettings,
	// Buttons
	buttonsSleepMode, 8,
	// Initialization function
	[](void) {
		sleepModeOn = false;
		sleepSettingHours = false;

		buttonsSleepMode[1].setForcePressed(sleepModeOn);

		sleepSetVisibilities();
	},
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(20, 70, FONT_TITLE, 0, "SleepMode");

		if (!sleepSettingHours) {
			GD.cmd_text(20, 185, FONT_LARGE, 0, LanguageString({
				"OPERATING HOURS",
				"BETRIEBSSTUNDEN",
				"HEURES DE FONCTIONNEMENT",
				"HORAS DE FUNCIONAMIENTO"
			})());
			GD.cmd_text(20, 215, FONT_LIGHT, 0, smWeekdays());
			GD.cmd_text(20, 235, FONT_LIGHT, 0, smWeekend());
		} else {
			GD.cmd_text(136, 130, FONT_TITLE, OPT_CENTER, LanguageString({
				"Set Operating Hours",
				"Set Betriebsstunden",
				"Programmer Heures de Fonctionnement",
				"Programar Horas de Funcionamiento"
			})());
			GD.cmd_text(136, 260, FONT_SMALL, OPT_CENTER, "TO");

			GD.cmd_text(136, 155, FONT_TITLE, OPT_CENTER,
				sleepSettingWeekday ? smWeekdays() : smWeekend());

			GD.ColorRGB(WHITE);
			GD.Begin(RECTS);
			GD.Vertex2ii(34, 244); GD.Vertex2ii(104, 282);
			GD.Vertex2ii(168, 244); GD.Vertex2ii(238, 282);
			GD.ColorRGB(BLACK);

			int i = sleepSettingWeekday ? 0 : 2;
			GD.cmd_text(69, 263, FONT_SMALL, OPT_CENTER,
				sleepHourString.format(sleepHours[i]));
			GD.cmd_text(203, 263, FONT_SMALL, OPT_CENTER,
				sleepHourString.format(sleepHours[i + 1]));
		}
	}
);

