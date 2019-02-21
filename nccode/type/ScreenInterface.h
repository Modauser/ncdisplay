#ifndef SCREENINTERFACE_H_
#define SCREENINTERFACE_H_

#include "ScreenID.h"

#include <array>
#include <cstddef>

class ScreenInterface;

class ScreenManager {
private:
	static std::array<ScreenInterface*,
		static_cast<std::size_t>(ScreenID::Count)> screens;
	static ScreenInterface *current;

public:
	static void addScreen(ScreenID id, ScreenInterface *screen);
	static void setCurrent(ScreenID id);
	static void showCurrent(void);
};

class ScreenInterface {
private:
	ScreenID parent;

public:
	constexpr ScreenInterface(ScreenID _this, ScreenID _parent)
		: parent(_parent) {
		ScreenManager::addScreen(_this, this);
	}

	virtual void prepare(void) = 0;
	virtual void show(void) = 0;
};

#endif // SCREENINTERFACE_H_
