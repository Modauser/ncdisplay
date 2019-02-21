#include "ScreenInterface.h"

ScreenInterface *ScreenManager::current;

std::array<ScreenInterface*, static_cast<std::size_t>(ScreenID::Count)>
	ScreenManager::screens;

void ScreenManager::addScreen(ScreenID id, ScreenInterface *screen) {
	screens[static_cast<std::size_t>(id)] = screen;
}

void ScreenManager::setCurrent(ScreenID id) {
	if (auto screen = screens[static_cast<std::size_t>(id)];
		current != screen) {
		current = screen;
		current->prepare();
	}
}

void ScreenManager::showCurrent(void)
{
	current->show();
}

