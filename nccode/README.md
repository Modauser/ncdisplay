## nccode
These files are most of what makes up the 'user' binary, which provides the
entire display interface.

### Files
| File                     | Description
| ------------------------ | -----------
| Advanced.cpp             | Screen: Advanced
| Dispense.cpp             | Screen: Dispense
| FillScreen.cpp           | Screen: FILLING ... WATER SYSTEM
| FilterChange.cpp         | Screen: Change Filter
| FilterChangeTimer.cpp    | Screen: Change Filter, set timer
| Filter.cpp               | Screen: Filter
| Language.cpp             | Screen: Language
| Lock.cpp                 | Screen: Passcode
| Lockscreen.h             | Screen: Enter Passcode
| Lockscreen.cpp           | Screen: Enter Passcode
| ProductInfo.cpp          | Screen: Product Info
| Restart.cpp              | Screen: Please Restart
| ServiceDetails.cpp       | Screen: Service Details
| SettingsMenu.cpp         | Screen: %Settings menu
| SetupComplete.cpp        | Screen: Setup Complete
| Setup.cpp                | Screen: ION logo
| Sleep.cpp                | Screen: Sleep screens (images, bottles saved)
| SleepMode.cpp            | Screen: SleepMode
| SystemOptions.cpp        | Screen: System Options
| Temperature.cpp          | Screen: Temperature
| TimeDate.cpp             | Screen: Time + Date
| USBUpload.cpp            | Screen: USB Upload
| Warning.cpp              | Screen: Red warning screen (factory reset/autofill)
| WelcomeLanguage.cpp      | Screen: Welcome... select language
| Errors.h                 | Main board error handling
| Errors.cpp               | Main board error handling
| Init.cpp                 | Initializes display, screens, and main board comm.
| MainBoard.h              | Main board communication 
| MainBoard.cpp            | Main board communication 
| Preinit.cpp              | Landing point from display power-up
| Settings.h               | Text file loading (labels/password) 
| Settings.cpp             | Text file loading (labels/password) 
| SystemCalls.h            | System/service calls to 'system' binary 
| SystemCalls.cpp          | System/service calls to 'system' binary 
| USBUpdate.cpp            | Checks for and loads firmware update from USB
| USBUpdateFiles.cpp       | Checks for and loads files from USB
| type/Assets.h            | Asset/Rendering variables/functions 
| type/Assets.cpp          | Asset/Rendering variables/functions 
| type/Button.h            | Button class for creating buttons 
| type/Button.cpp          | Button class for creating buttons 
| type/Formatters.h        | Utilities for formatting numbers as strings 
| type/Formatters.cpp      | Utilities for formatting numbers as strings 
| type/LanguageString.h    | Assists in loading text for set language 
| type/LanguageString.cpp  | Assists in loading text for set language 
| type/NirmalaMod.h        | Data for customized fonts on SD card
| type/Screen.h            | Provides a "Screen" object
| type/ScreenID.h          | List of ScreenID's for every Screen
| type/ScreenInterface.h   | Classes for managing Screens
| type/ScreenInterface.cpp | Classes for managing Screens
| type/SpecialCharacters.h | Constants for font indices of special characters

