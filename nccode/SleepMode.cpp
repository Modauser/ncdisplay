/**
 * @class SleepMode.cpp
 * @brief Screen for configuring SleepMode.
 */
#include "type/Assets.h"
#include "type/Formatters.h"
#include "type/Screen.h"
#include "MainBoard.h"
#include "Settings.h"

#include <gameduino2/GD2.h>

static bool sleepModeOn = false;
static bool sleepSettingHours = false;
static bool sleepSettingWeekday = false;

static AMPMFormat sleepHourString;

static unsigned int sleepHours[4] = {
	0, 0, 0, 0
};

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

static void updateAfterToggle(void);
static void sleepSetVisibilities(void);
static void updateSaveButtonText(void);

static Screen SleepMode (
	ScreenID::SleepMode,
	// Parent screen
	ScreenID::Settings,
	// Initialization function
	[](void) {
		MainBoard::getSleepmodeHours(sleepHours);
		sleepModeOn = MainBoard::getSleepmodeEnabled();
		sleepSettingHours = false;

		updateAfterToggle();
		sleepSetVisibilities();
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.Begin(RECTS);
		GD.Vertex2ii(0, 120);
		GD.Vertex2ii(272, 120);

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(20, 90, FONT_TITLE, OPT_CENTERY, "SleepMode");

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
			GD.cmd_text(136, 150, FONT_TITLE, OPT_CENTER, LanguageString({
				"Set Operating Hours",
				"Set Betriebsstunden",
				"Programmer Heures de Fonctionnement",
				"Programar Horas de Funcionamiento"
			})());
			GD.cmd_text(136, 260, FONT_SMALL, OPT_CENTER, "TO");

			GD.cmd_text(136, 180, FONT_TITLE, OPT_CENTER,
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
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press) {
			if (sleepSettingHours) {
				if (!sleepSettingWeekday)
					sleepSettingWeekday = true;
				else
					sleepSettingHours = false;

				sleepSetVisibilities();
			} else {
				ScreenManager::setCurrent(ScreenID::Settings);
			}
		}
	}),
	Button({180, 85}, Button::drawToggle, [](bool press) {
		if (!press) {
			sleepModeOn ^= true;
			updateAfterToggle();
		}
	}),
	Button({0, 120}, Button::drawMenuItem, {
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
	Button({0, 420}, Button::drawFullWidth, lStringSave, [](bool press) {
		if (press)
			return;

		if (sleepSettingHours & sleepSettingWeekday) {
			sleepSettingWeekday = false;
			updateSaveButtonText();
		} else {
			MainBoard::setSleepmodeHours(sleepHours);
			MainBoard::setSleepmodeEnabled(sleepModeOn);
			ScreenManager::setCurrent(ScreenID::Settings);
		}
	}),
	Button({34, 210}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		auto& hour = sleepHours[sleepSettingWeekday ? 0 : 2];
		if (hour < 23)
			hour++;
		else
			hour = 0;
	}),
	Button({34, 282}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		auto& hour = sleepHours[sleepSettingWeekday ? 0 : 2];
		if (hour > 0)
			hour--;
		else
			hour = 23;
	}),
	Button({168, 210}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		auto& hour = sleepHours[sleepSettingWeekday ? 1 : 3];
		if (hour < 23)
			hour++;
		else
			hour = 0;
	}),
	Button({168, 282}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		auto& hour = sleepHours[sleepSettingWeekday ? 1 : 3];
		if (hour > 0)
			hour--;
		else
			hour = 23;
	})
);

void updateAfterToggle(void)
{
	SleepMode.getButton(1).setForcePressed(sleepModeOn);
	SleepMode.getButton(2).setVisibility(sleepModeOn);
}

void sleepSetVisibilities(void)
{
	SleepMode.getButton(1).setVisibility(!sleepSettingHours);
	SleepMode.getButton(2).setVisibility(sleepModeOn & !sleepSettingHours);
	SleepMode.getButton(4).setVisibility(sleepSettingHours);
	SleepMode.getButton(5).setVisibility(sleepSettingHours);
	SleepMode.getButton(6).setVisibility(sleepSettingHours);
	SleepMode.getButton(7).setVisibility(sleepSettingHours);

	updateSaveButtonText();
}

void updateSaveButtonText(void)
{
	SleepMode.getButton(3).setText((sleepSettingHours & sleepSettingWeekday) ?
		lStringNext : lStringSave);
}


