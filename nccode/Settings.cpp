/**
 * @file Settings.h
 * @brief Display-wide settings.
 */
#include "MainBoard.h"
#include "Settings.h"

#include <fatfs/ff.h>

std::array<char[Settings::LabelSize], 23> Settings::Labels = {
};
char Settings::Password[4] = { '1', '2', '3', '4' };

static const char *PasswordFile = DRV_SD "Password.txt";

static const LanguageString SettingsFile ({
	DRV_SD "EngSet01.txt",
	DRV_SD "GerSet00.txt",
	DRV_SD "FreSet00.txt",
	DRV_SD "SpaSet00.txt"
});

static const LanguageString SettingsFileMetric ({
	DRV_SD "EngSetL0.txt",
	DRV_SD "GerSetL0.txt",
	DRV_SD "FreSetL0.txt",
	DRV_SD "SpaSetL0.txt"
});

void Settings::loadLabels(void)
{
	FIL fd;

	const auto& file = MainBoard::isMetric() ? SettingsFileMetric
		: SettingsFile;
	if (auto r = f_open(&fd, file(), FA_READ); r != FR_OK)
		return;

	for (unsigned int i = 0; i < Labels.size(); i++)
		f_gets(Labels[i], LabelSize, &fd);

	f_close(&fd);
}

void Settings::loadPassword(void)
{
	FIL fd;

	if (auto r = f_open(&fd, PasswordFile, FA_READ); r != FR_OK)
		return;

	UINT bytesRead;
	f_read(&fd, Password, 4, &bytesRead);
	f_close(&fd);
}

bool Settings::isCorrectPassword(const char *guess)
{
	for (unsigned int i = 0; i < 4; i++) {
		if (guess[i] != Password[i])
			return false;
	}

	return true;
}

