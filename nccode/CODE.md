## Code guide

The files in this directory make up the User part of ncdisplay, which provides
the user-interface and handles main board communication.

`README.md` describes what each file does. This file will describe how the code
here 'works'.

---

Execution begins in `Preinit.cpp`, which only does low-level things like
variable initialization before calling `setup()` and `loop()` in `Init.cpp`.
`Init.cpp` does asset and config file loading, main board handshaking, and USB
update checks before loading the first screen.

'Screens' that are shown on the display are defined in this folder. Screens can
render text and buttons, and do communication with the main board. A screen
definition looks like this:
```
static Screen USBUpload (
	// Our ID
	ScreenID::USBUpload,
	// Parent screen's ID
	ScreenID::Advanced,
	// Initialization function
	[](void) {
		loadImage(FREE_HANDLE, "usb.jpg");
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.Begin(BITMAPS);
		GD.Vertex2ii(112, 196, FREE_HANDLE);

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 150, FONT_TITLE, OPT_CENTERX, LanguageString({
			"1. Connect USB",
			"1. USB Verbinden",
			"1. Connecter la cl" e_ACUTE " USB",
			"1. Conectar USB"
		})());
		GD.cmd_text(136, 300, FONT_TITLE, OPT_CENTERX, LanguageString({
			"2. Restart ION to upload",
			"2. Neustart ION zum\nHochladen",
			"2. Red" e_ACUTE "marrer ION pour\nt" e_ACUTE "l" e_ACUTE
			"charger",
			"2. Rencender el ION para\nsubir la informaci" o_ACUTE
			"n"
		})());
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::Advanced);
	})
);
```
At the beginning of the Screen's definition is its ID (`ScreenID::USBUpload`).
IDs are defined in `type/ScreenID.h`, and every Screen has an ID. Following the
Screen's ID in the code is its parent's ID; when the display times out from no
interaction, the parent screen is loaded. At the top-level screen (Dispense),
the parent is a sleep screen that shows the sleep images.

After the ID and parent ID is the screen initialization function, called before
the screen is shown. This function may be used to do things like load assets or
load data from the main board.

Next is a 'pre-draw' function, which is called at the start of the display
rendering loop. Here is where Screens color the background or show text and
images. Screens may also do other things that should be run in a loop, like
run an error check.

Beyond the pre-draw function are button definitions (or nothing if there are no
buttons). Each button has an xy coordinate (`{0, 0}` in the example), render
function, optional text and a function that is called every press or release.
See `type/Button.h` for more info.

Finally, Screens including the one in the example above use `LanguageString`
objects. These objects contain text definitions for each language, in the order
given in `type/LanguageString.h`. By calling a LanguageString as a function
(having `()` at the object's end), the text for the current language is
automatically selected, with a default to English.

