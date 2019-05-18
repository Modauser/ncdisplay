/**
 * @file Setup.cpp
 * @brief The first, large ION logo screen seen on startup.
 */
#include "type/Assets.h"
#include "type/Screen.h"

#include <gameduino2/GD2.h>

static unsigned int holdCounter = 0;

static Screen SetupComplete (
	// Our ID
	ScreenID::Setup,
	// Parent screen
	ScreenID::Dispense,
	// Initialization function
	[](void) {
		holdCounter = 0;
		loadImage(FREE_HANDLE, "startup.jpg");
	},
	// Pre-draw function
	[](void) {
		GD.ClearColorRGB(NC_BKGND_COLOR);
		GD.Clear();

		GD.Begin(BITMAPS);
		GD.Vertex2ii(0, 120, FREE_HANDLE);

		// 5 second timeout to next
		if (++holdCounter >= 500)
			ScreenManager::setCurrent(ScreenID::Fill);
	}
);


