#include "ScreenInterface.h"

ScreenInterface *ScreenManager::current;
ScreenInterface *ScreenManager::forcedParent = nullptr;

std::array<ScreenInterface*, static_cast<std::size_t>(ScreenID::Count)>
	ScreenManager::screens;

void ScreenManager::addScreen(ScreenID id, ScreenInterface *screen) {
	screens[static_cast<std::size_t>(id)] = screen;
}

void ScreenManager::setCurrent(ScreenID id) {
	if (forcedParent != nullptr) {
		current = forcedParent;
		current->prepare();
		forcedParent = nullptr;
	} else if (auto screen = screens[static_cast<std::size_t>(id)];
		current != screen) {
		current = screen;
		current->prepare();
	}
}

void ScreenManager::setCurrent(ScreenID id, ScreenID fpid)
{
	setCurrent(id);
	forcedParent = screens[static_cast<std::size_t>(fpid)];
}

void ScreenManager::showCurrent(void)
{
	current->show();
}

