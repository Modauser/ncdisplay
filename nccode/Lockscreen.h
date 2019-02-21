/**
 * @file Lockscreen.h
 * @brief Provides lockscreen functionality.
 */
#ifndef LOCKSCREEN_H_
#define LOCKSCREEN_H_

#include "type/ScreenID.h"

/**
 * If lockscreen is enabled, presents it before allowing access to the given
 * screen.
 * If lockscreen is  disabled, sets screen to the current screen.
 */
void doPasscodeTest(ScreenID screen);

#endif // LOCKSCREEN_H_
