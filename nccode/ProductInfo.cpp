#include <fatfs/ff.h>
#include <gameduino2/GD2.h>
#include <stdio.h>

#include "Assets.h"
#include "Settings.h"
#include "Screens.h"

constexpr const char *piServiceFile = DRV_SD "Service.txt";

static Button buttonsProductInfo[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			screenCurrent = &screenSettings;
	})
};

static unsigned int piModel;
static char piSerial[16];
static char piSoftware[] = { 0, '.', 0, '\0' };
static char piFlowRate[] = { 0, 0, '.', 0, ' ', 0, 0, 0, '\0' };

// Shared with Filter.cpp
const char *piFilterTypes[] = {
	"CarbonPlus", "CarbonPro", "FiberTek", "CarbonPhos", "CarbonSilv",
	"Unknown"
};
// Shared with Filter.cpp
unsigned int piFilterType = 5;

static unsigned int piFilterRemaining;
//static unsigned int piFilterFlowRate;
static char piServiceContact[200];

Screen screenProductInfo (
	// Parent screen
	&screenSettings,
	// Buttons
	buttonsProductInfo, 1,
	// Initialization function
	[](void) {
#ifdef USE_SERIAL
		serialPrintf("@!");
		piModel = serialGet();

		serialPrintf("@E");
		for (int i = 0; i < 15; i++)
			piSerial[i] = serialGet();
		piSerial[15] = '\0';

		serialPrintf("@#");
		piSoftware[0] = '0' + serialGet();
		piSoftware[2] = '0' + serialGet();

		serialPrintf("@%%");
		piFilterType = serialGet();

		serialPrintf("@&");
		piFilterRemaining = serialGet() << 8;
		piFilterRemaining |= serialGet();

		serialPrintf("@F");
		unsigned int val = serialGet();
		piFlowRate[3] = (val % 10) + '0';
		val /= 10;
		piFlowRate[1] = (val % 10) + '0';
		val /= 10;
		piFlowRate[0] = (val == 0) ? ' ' : (val % 10) + '0';
		
		serialPrintf("@X");
		if (serialGet() != 0) {
			piFlowRate[5] = 'L';
		} else {
			piFlowRate[5] = 'G';
			piFlowRate[6] = 'P';
			piFlowRate[7] = 'M';
		}
#endif // USE_SERIAL

		FIL fil;
		auto result = f_open(&fil, piServiceFile, FA_READ);
		if (result == FR_OK) {
			UINT unused;
			f_read(&fil, piServiceContact, 200, &unused);
			unsigned int i;
			for (i = 0; piServiceContact[i]; i++) {
				if (piServiceContact[i] == '\r')
					piServiceContact[i] = ' ';
			}
			f_close(&fil);
		}
	},
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(20, 80, FONT_LARGE, 0, LanguageString({
			"PRODUCT INFO",
			"INFO",
			"INFO",
			"INFO"
		})());

		GD.cmd_text(20,  110, FONT_SMALL, 0, Settings::getLabel(0));
		GD.cmd_text(21,  130, FONT_SMALL, 0, Settings::getLabel(1));
		GD.cmd_text(20,  150, FONT_SMALL, 0, Settings::getLabel(2));

	      GD.cmd_number(140, 110, FONT_SMALL, 0, piModel);
		GD.cmd_text(140, 130, FONT_SMALL, 0, piSerial);
		GD.cmd_text(140, 150, FONT_SMALL, 0, piSoftware);

		GD.cmd_text(20, 190, FONT_LARGE, 0, LanguageString({
			"FILTER INFO",
			"FILTERINFO",
			"INFORMATION DU FILTRE",
			"INFORMACION DEL FILTRO"
		})());

		GD.cmd_text(20,  220, FONT_SMALL, 0, Settings::getLabel(3));
		GD.cmd_text(20,  240, FONT_SMALL, 0, Settings::getLabel(4));
		GD.cmd_text(20,  260, FONT_SMALL, 0, Settings::getLabel(5));

	        GD.cmd_text(140, 220, FONT_SMALL, 0, piFilterTypes[piFilterType]);
	      GD.cmd_number(140, 240, FONT_SMALL, 0, piFilterRemaining);
		GD.cmd_text(140, 260, FONT_SMALL, 0, piFlowRate);

		GD.cmd_text(20, 380, FONT_SMALL, 0, piServiceContact, 20);
	}
);

