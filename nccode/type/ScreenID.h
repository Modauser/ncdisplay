/**
 * @file ScreenID.h
 * @brief A list of IDs for each screen.
 */
#ifndef SCREENID_H_
#define SCREENID_H_

/**
 * @enum ScreenID
 * Gives a ID/handle to each screen.   
 * 
 * To switch screens, the desired screen's ID must be known.
 * Count is used to size the array of known screens.
 * @see ScreenInterface
 */
enum class ScreenID {
	Sleep = 0,         /**< The sleep image screen */
	Dispense,          /**< Dispense screen */
	Settings,          /**< %Settings menu */
	Settings100,		/**< %Settings menu without CO2 */
	ProductInfo,       /**< Product info */
	Filter,            /**< Filter no CO2 */
	FilterChange,      /**< Filter change  */
	Filter100,            /**< Filter */
	FilterChange100,      /**< Filter change no CO2*/
	CO2Level,	 	   /**< Shows the level of the CO2 tank */	
	CO2Tank,		   /**< Selects the CO2 tank */		
	FilterChangeTimer, /**< Filter change, timer screen */
	FilterChangeTimer100, /**< Filter change, timer screen no CO2*/
	TimeDate,          /**< Time + Date */
	Language,          /**< Language (set language) */
	SleepMode,         /**< SleepMode */
	Temperature,       /**< Temperature */
	Advanced,          /**< Advanced menu */
	USBUpload,         /**< USB Upload */
	Warning,           /**< Red warning (factory reset/autofill) */
	ServiceDetails,    /**< Service details */
	SystemOptions,     /**< System options */
	Lock,              /**< Passcode */
	Lockscreen,        /**< Enter passcode prompt */
	Restart,           /**< Please restart */
	Fill,              /**< FILLING ... WATER SYSTEM */
	WelcomeLanguage,   /**< Welcome, select language */
	SetupComplete,     /**< Setup complete */
	Setup,             /**< Fullscreen ION logo */
	SystemError,       /**< System error */

	Count              /**< Count of defined ScreenID's */
};

#endif // SCREENID_H_

