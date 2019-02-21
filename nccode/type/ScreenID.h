/**
 * @file ScreenID.h
 * @brief A list of IDs for each screen.
 */
#ifndef SCREENID_H_
#define SCREENID_H_

/**
 * Gives a handle for each screen, used to help link them together.
 * Count is used to size the array of known screens.
 */
enum class ScreenID {
	Sleep = 0,
	MainMenu,
	Dispense,
	Settings,
	ProductInfo,
	Filter,
	FilterChange,
	FilterChangeTimer,
	TimeDate,
	Language,
	SleepMode,
	Temperature,
	Advanced,
	USBUpload,
	Warning,
	ServiceDetails,
	SystemOptions,
	Lock,
	Lockscreen,
	Restart,
	Fill,
	WelcomeLanguage,

	Count
};

#endif // SCREENID_H_

