#include "Settings.h"

#include <fatfs/ff.h>

std::array<char[50], 23> Settings::Labels = {
};

static const LanguageString SettingsFile ({
	DRV_SD "EngSet01.txt",
	DRV_SD "GerSet00.txt",
	DRV_SD "FreSet00.txt",
	DRV_SD "SpaSet00.txt"
});

void Settings::loadLabels(void)
{
	FIL fd;

	if (auto r = f_open(&fd, SettingsFile(), FA_READ); r != FR_OK)
		return;

	for (unsigned int i = 0; i < Labels.size(); i++)
		f_gets(Labels[i], 50, &fd);

	f_close(&fd);
}

