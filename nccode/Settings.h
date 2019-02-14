#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "LanguageString.h"

#include <array>

class Settings {
private:
	static std::array<char[50], 23> Labels;

public:
	static void loadLabels(void);

	inline static const char *getLabel(unsigned int index) {
		return Labels[index];
	}
};

#endif // SETTINGS_H_

