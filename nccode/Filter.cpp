#include <gameduino2/GD2.h>
#include <stdio.h>

#include "Assets.h"
#include "Screens.h"

static Button buttonsFilter[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			screenCurrent = &screenSettings;
	}),
	Button(2, {0, 422}, Button::drawFullWidth, {
		"CHANGE FILTER",
		"FILTER WECHSELN",
		"CHANGER LE FILTRE",
		"CAMBIAR EL FILTRO"
	}, [](bool press) {
		if (!press)
			screenCurrent = &screenFilterChange;
	})
};

extern const char *piFilterTypes[];
extern unsigned int piFilterType;

static const char *filterReorder[] = {
	"5335",
	"6905",
	"6906",
	"5341",
	"Unknown"
};

static unsigned int filterGalsRem = 0;
static unsigned int filterMonthsRem = 0;
static char filterLastChanged[] = { 0, 0, '/', 0, 0, '/', 0, 0, '\0' };
static bool filterMetric;

Screen screenFilter (
	// Parent screen
	&screenSettings,
	// Buttons
	buttonsFilter, 2,
	// Initialization function
	[](void) {
#ifdef USE_SERIAL
		printf("@%%");
		piFilterType = serialGet();

		printf("@&");
		filterGalsRem = serialGet() << 8;
		filterGalsRem |= serialGet();

		if (filterMetric)
			filterGalsRem *= GAL_TO_L;

		printf("@`");
		filterMonthsRem = serialGet();

		printf("@(");
		int val = serialGet();
		filterLastChanged[0] = val / 10 + '0';
		filterLastChanged[1] = val % 10 + '0';
		val = serialGet();
		filterLastChanged[3] = val / 10 + '0';
		filterLastChanged[4] = val % 10 + '0';
		val = serialGet();
		filterLastChanged[6] = val / 10 + '0';
		filterLastChanged[7] = val % 10 + '0';

		printf("@X");
		filterMetric = (serialGet() != 0);
#else
		piFilterType = 0;
	},
#endif // USE_SERIAL
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(20, 80, FONT_LARGE, 0, LanguageString({
			"FILTER INFO",
			"FILTERINFO",
			"INFORMATIONS SUR LE FILTRE",
			"INFOMACI" o_ACUTE "N DEL FILTRO"
		})());

		GD.cmd_text(20,  110, FONT_SMALL, 0, LanguageString({
			"Current Type:",
			"Jetziger Typ:",
			"Type:",
			"Tipo:"
		})());
		GD.cmd_text(20,  130, FONT_SMALL, 0, LanguageString({
			"Re-Order Number:",
			"Nummer:",
			"Num" e_ACUTE "ro:",
			"Numero para pedido:"
		})());
		GD.cmd_text(20,  150, FONT_SMALL, 0, (filterMetric ? 
			LanguageString({
				"Liters Remaining:",
				"Verbleibende Liter:",
				"Litres restants:",
				"Litros quedantes:"
			}) :
			LanguageString({
				"Gallons Remaining:",
				"Verbleibende Gallon:",
				"Gallons restants:",
				"Gallons quedantes:"
			})
		)());
		GD.cmd_text(20,  170, FONT_SMALL, 0, LanguageString({
			"Months Remaining:",
			"Verbleibende Monate:",
			"Mois restants:",
			"Meses quedantes:"
		})());
		GD.cmd_text(20,  190, FONT_SMALL, 0, LanguageString({
			"Last Filter Change:",
			"Letzter Filterwechsel:",
			"Dernier changement",
			"Ultimo cambio:"
		})());

		GD.cmd_text(180, 110, FONT_SMALL, 0, piFilterTypes[piFilterType]);
		GD.cmd_text(180, 130, FONT_SMALL, 0, filterReorder[piFilterType]);
	      GD.cmd_number(180, 150, FONT_SMALL, 0, filterGalsRem);
	      GD.cmd_number(180, 170, FONT_SMALL, 0, filterMonthsRem);
		GD.cmd_text(180, 190, FONT_SMALL, 0, filterLastChanged);

		GD.cmd_text(20, 230, FONT_LARGE, 0, LanguageString({
			"CONTAMINANTS REMOVED",
			"ENTFERNTE",
			"CONTAMINANTS " E_ACUTE "LIMIN" e_ACUTE "S",
			"CONTAMINANTES ELIMINADOS"
		})());
		GD.cmd_text(20, 260, FONT_SMALL, 0, LanguageString({
			"Chlorine, Taste, Odor, Lead, Cysts",
			"Chlor, Geschmack, Geruch, Blei, Zysten",
			"Chlore, Go" u_HAT "t, Odeur, Plomb, Kystes",
			"Clorina, Sabor, Olor, Plomo, Quistes"
		})());
	}
);

