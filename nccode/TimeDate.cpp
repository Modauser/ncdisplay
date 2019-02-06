#include <gameduino2/GD2.h>

#include <stdio.h>

#include "Assets.h"
#include "Screens.h"

static bool ampm = true;
static int timeDate[] = {
	8, // Hour
	0, // Minute
	0, // AM/PM
	1, // Month
	1, // Day
	0  // Year
};

static bool timeSetting = true;

static int daysInMonth(int month, int year);

static Button buttonsTimeDate[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool press) {
		if (press)
			return;

		if (!timeSetting) {
			timeSetting = true;
			buttonsTimeDate[5].setVisibility(ampm);
			buttonsTimeDate[6].setVisibility(ampm);
			buttonsTimeDate[7].setVisibility(true);
			buttonsTimeDate[8].setVisibility(true);
			buttonsTimeDate[9].setText(lStringNext);
		} else {
			screenCurrent = &screenSettings;
		}
	}),
	Button(2, {14, 190}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		if (timeSetting) {
			++timeDate[0];
			if (ampm) {
				if (timeDate[0] > 12)
					timeDate[0] = 1;
			} else if (timeDate[0] > 23) {
				timeDate[0] = 0;
			}
		} else {
	       		if (++timeDate[3] > 12)
				timeDate[3] = 1;
			timeDate[4] = std::min(timeDate[4], daysInMonth(
				timeDate[3], timeDate[5]));
		}
	}),
	Button(3, {14, 262}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		if (timeSetting) {
			--timeDate[0];
			if (ampm) {
				if (timeDate[0] < 1)
					timeDate[0] = 12;
			} else if (timeDate[0] < 0) {
				timeDate[0] = 23;
			}
		} else {
			if (--timeDate[3] < 1)
				timeDate[3] = 12;
			timeDate[4] = std::min(timeDate[4], daysInMonth(
				timeDate[3], timeDate[5]));
		}
	}),
	Button(4, {100, 190}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		if (timeSetting && ++timeDate[1] > 59) {
			timeDate[1] = 0;
		} else {
			int days = daysInMonth(timeDate[3], timeDate[5]);
			if (++timeDate[4] > days)
				timeDate[4] = 1;
		}
	}),
	Button(5, {100, 262}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		if (timeSetting && --timeDate[1] < 0) {
			timeDate[1] = 59;
		} else {
			if (--timeDate[4] < 1) {
				timeDate[4] = daysInMonth(timeDate[3],
					timeDate[5]);
			}
		}
	}),
	Button(6, {186, 190}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		if (timeSetting) {
			timeDate[2] ^= 1;
		} else {
			if (++timeDate[5] > 99)
				timeDate[5] = 0;
			timeDate[4] = std::min(timeDate[4], daysInMonth(
				timeDate[3], timeDate[5]));
		}
	}),
	Button(7, {186, 262}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		if (timeSetting) {
			timeDate[2] ^= 1;
		} else {
			if (--timeDate[5] < 0)
				timeDate[5] = 99;
			timeDate[4] = std::min(timeDate[4], daysInMonth(
				timeDate[3], timeDate[5]));
		}
	}),
	Button(8, {14, 120}, Button::drawExclusiveOption, "AM/PM", [](bool press) {
		if (press)
			return;

		ampm = true;
		timeDate[2] = timeDate[0] > 12;
		if (timeDate[0] > 12)
			timeDate[0] -= 12;
		buttonsTimeDate[7].setForcePressed(ampm);
		buttonsTimeDate[8].setForcePressed(!ampm);
		buttonsTimeDate[5].setVisibility(ampm);
		buttonsTimeDate[6].setVisibility(ampm);
	}),
	Button(9, {134, 120}, Button::drawExclusiveOption, "24 HR", [](bool press) {
		if (press)
			return;

		ampm = false;
		if (timeDate[2] != 0 && timeDate[0] < 12)
			timeDate[0] += 12;
		buttonsTimeDate[7].setForcePressed(ampm);
		buttonsTimeDate[8].setForcePressed(!ampm);
		buttonsTimeDate[5].setVisibility(ampm);
		buttonsTimeDate[6].setVisibility(ampm);
	}),
	Button(10, {0, 422}, Button::drawFullWidth, lStringNext,
	[](bool press) {
		if (!press) {
			if (timeSetting) {
				timeSetting = false;
				buttonsTimeDate[5].setVisibility(true);
				buttonsTimeDate[6].setVisibility(true);
				buttonsTimeDate[7].setVisibility(false);
				buttonsTimeDate[8].setVisibility(false);
				buttonsTimeDate[9].setText(lStringSave);
			} else {
#ifdef USE_SERIAL
				printf("@.%02u%02u%02u",
					timeDate[3], timeDate[4], timeDate[5]);
				if (ampm) {
					printf("@<%02u%02u%01u", timeDate[0],
						timeDate[1], timeDate[2]);
				} else {
					printf("@-%02u%02u", timeDate[0],
						timeDate[1]);
				}
#endif // USE_SERIAL
				screenCurrent = &screenSettings;
			}
		}
	})
};

static const LanguageString tdSetTime ({
	"Set Time",
	"Zeit",
	"R" e_ACUTE "glage de L'Heure",
	"Programar Hora"
});
static const LanguageString tdSetDate ({
	"Set Date",
	"Datum",
	"R" e_ACUTE "glage de la Date",
	"Programar Fecha"
});
static const LanguageString tdMonth ({
	"MM",
	"MM",
	"MM",
	"MM"
});
static const LanguageString tdDay ({
	"DD",
	"TT",
	"JJ",
	"DD"
});
static const LanguageString tdYear ({
	"YY",
	"JJ",
	"AA",
	"AA"
});

Screen screenTimeDate (
	// Parent screen
	&screenSettings,
	// Buttons
	buttonsTimeDate, 10,
	// Initialization function
	[](void) {
		timeSetting = true;
		ampm = true;

#ifdef USE_SERIAL
		printf("@T");
		timeDate[0] = serialGet(); // Hour
		timeDate[1] = serialGet(); // Minute
		serialGet(); // Second, unused
		timeDate[2] = serialGet(); // AM/PM
		ampm = timeDate[2] < 2;
	
		printf("@D");
		timeDate[3] = serialGet(); // Month
		timeDate[4] = serialGet(); // Day
		timeDate[5] = serialGet(); // Year
#endif // USE_SERIAL

		buttonsTimeDate[5].setVisibility(ampm);
		buttonsTimeDate[6].setVisibility(ampm);
		buttonsTimeDate[7].setVisibility(true);
		buttonsTimeDate[8].setVisibility(true);
		buttonsTimeDate[9].setText(lStringNext);

		buttonsTimeDate[7].setForcePressed(ampm);
		buttonsTimeDate[8].setForcePressed(!ampm);
	},
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 70, FONT_TITLE, OPT_CENTERX, timeSetting ?
			tdSetTime() : tdSetDate());

		if (!timeSetting) {
			GD.cmd_text(49, 164, FONT_SMALL, OPT_CENTER, tdMonth());
			GD.cmd_text(92, 164, FONT_SMALL, OPT_CENTER, "/");
			GD.cmd_text(135, 164, FONT_SMALL, OPT_CENTER, tdDay());
			GD.cmd_text(178, 164, FONT_SMALL, OPT_CENTER, "/");
			GD.cmd_text(221, 164, FONT_SMALL, OPT_CENTER, tdYear());
		}

		GD.ColorRGB(WHITE);
		GD.Begin(RECTS);
		GD.Vertex2ii(14, 224); GD.Vertex2ii(84, 258);
		GD.Vertex2ii(100, 224); GD.Vertex2ii(170, 258);
		if (!timeSetting || ampm) {
			GD.Vertex2ii(186, 224);
			GD.Vertex2ii(256, 258);
		}

		int idx = timeSetting ? 0 : 3;
		GD.ColorRGB(BLACK);
		GD.cmd_number(49, 244, FONT_SMALL, OPT_CENTER,
			timeDate[idx]);
		GD.cmd_number(135, 244, FONT_SMALL, OPT_CENTER,
			timeDate[idx + 1]);
		if (timeSetting) {
			if (ampm) {
				GD.cmd_text(221, 244, FONT_SMALL, OPT_CENTER,
					timeDate[idx + 2] ? "PM" : "AM");
			}
		} else {
			GD.cmd_number(221, 244, FONT_SMALL, OPT_CENTER,
				timeDate[idx + 2]);
		}
	}
);

int daysInMonth(int month, int year)
{
	bool isLeapYear;
	switch (month) {
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
		break;
	case 2:
		isLeapYear = ((year % 4) == 0 && (year % 100) != 0) ||
			((year % 400) == 0);
		return 28 + isLeapYear;
		break;
	default:
		return 31;
		break;
	}
}

