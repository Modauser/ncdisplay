/**
 * @file ScreenInterface.h
 * @brief Provides a base screen definition and a class to manage screens.
 */

#ifndef SCREENINTERFACE_H_
#define SCREENINTERFACE_H_

#include "ScreenID.h"

#include <array>
#include <cstddef>

class ScreenInterface;

/**
 * @class ScreenManager
 * @brief Manages all known screens.
 */
class ScreenManager {
private:
	/**
	 * An array to accomodate all known screens; the array size is based on
	 * the count of ScreenID's.
	 */
	static std::array<ScreenInterface*,
		static_cast<std::size_t>(ScreenID::Count)> screens;

	/**
	 * Pointer to the current screen.
	 */
	static ScreenInterface *current;

	/**
	 * Pointer to the parent forced on the current screen, set to nullptr if
	 * no parent is being forced.
	 */
	static ScreenInterface *forcedParent;

public:
	/**
	 * Fills a screen entry with the given screen.
	 * @param id The ID of the screen being added
	 * @param screen The screen object being added
	 */
	static void addScreen(ScreenID id, ScreenInterface *screen);

	/**
	 * Sets the current screen based on the given ID.
	 * @param id The ID of the screen being loaded
	 */
	static void setCurrent(ScreenID id);

	/**
	 * Sets the current screen based on the ID, and overrides the screen's
	 * parent.
	 * @param id The screen to load
	 * @param forcedParent The parent screen to force on the loaded screen
	 */
	static void setCurrent(ScreenID id, ScreenID forcedParent);

	/**
	 * Renders the current screen.
	 */
	static void showCurrent(void);
};

/**
 * @class ScreenInterface
 * @brief The base of a Screen.
 */
class ScreenInterface {
protected:
	/**
	 * ID of the screen's parent.
	 */
	ScreenID parent;

public:
	/**
	 * Creates a screen with the given values, and adds it to the screen
	 * manager.
	 * @param _this This screen's ID
	 * @param _parent This screen's parent's ID
	 */
	constexpr ScreenInterface(ScreenID _this, ScreenID _parent)
		: parent(_parent) {
		ScreenManager::addScreen(_this, this);
	}

	/**
	 * Prepares the screen; a one-time initialization function.
	 */
	virtual void prepare(void) = 0;

	/**
	 * Called to render the screen.
	 */
	virtual void show(void) = 0;
};

#endif // SCREENINTERFACE_H_
