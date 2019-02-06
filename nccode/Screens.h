/**
 * @file Screens.h
 * @brief Exposes all available screens.
 */
#ifndef SCREENS_H_
#define SCREENS_H_

class Screen;

extern Screen screenDispense;
extern Screen screenSettings;

extern Screen screenSleep;

// Settings screen children
extern Screen screenProductInfo;
extern Screen screenFilter;
extern Screen screenTimeDate;
extern Screen screenLanguage;
extern Screen screenSleepMode;
extern Screen screenTemperature;
extern Screen screenAdvanced;

extern Screen screenFilterChange;
extern Screen screenFilterChangeTimer;

// Advanced screen children
extern Screen screenUSBUpload;
//extern Screen screenFactoryReset;
extern Screen screenServiceDetails;
extern Screen screenSystemOptions;
extern Screen screenLock;
//extern Screen screenAutofill;

extern Screen screenWarning;

// Passcode entry screen
extern Screen screenLockscreen;
void doPasscodeTest(Screen *s);

#endif // SCREENS_H_

