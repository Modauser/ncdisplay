#include <gameduino2/GD2.h>
#include "Assets.h"
#include "Screens.h"

#include "NirmalaMod.h"

// Load extra images right after the gd3asset file
static uint32_t imageNextAddress = ASSETS_END;
void loadImage(int handle, const char *path)
{
  uint32_t base, width, height;

  imageNextAddress = (imageNextAddress + 1024) & ~1023;

  GD.BitmapHandle(handle);
  GD.cmd_loadimage(imageNextAddress, 0);
  GD.load(path);
  GD.cmd_getprops(base, width, height);
  GD.finish();
  
  imageNextAddress = base + width * height * 2;
}

void displayInit(void)
{
  GD.begin();

  // Prepare display
  GD.wr32(REG_HSIZE, 480);
  GD.wr32(REG_VSIZE, 272);
  GD.wr32(REG_HCYCLE, 548);
  GD.wr32(REG_HOFFSET, 43);
  GD.wr32(REG_HSYNC0, 0);
  GD.wr32(REG_HSYNC1, 41);
  GD.wr32(REG_VCYCLE, 292);
  GD.wr32(REG_VOFFSET, 12);
  GD.wr32(REG_VSYNC0, 0);
  GD.wr32(REG_VSYNC1, 10);
  GD.wr32(REG_PCLK, 5);
  GD.wr32(REG_PCLK_POL, 1);
  GD.wr32(REG_CSPREAD, 1);
  GD.wr32(REG_DITHER, 1);
  GD.wr32(REG_ROTATE, 2);
  GD.wr(REG_SWIZZLE, 0);
}

uint32_t fontLoad(int handle, const byte *header, const char *datafile, uint32_t address)
{
	// 4-byte alignment necessary for header
	address = (address + 3) & ~(3);

	GD.BitmapHandle(handle);
	uint32_t fileSize = GD.loadMem(datafile, address + 148);
	GD.cmd_setbitmap(address + 148, header[128], header[136], header[140]);

	GD.wr_n(address, (byte *)header, 148);
	GD.cmd_setfont(handle, address);
	return address + 148 + fileSize;
}

void loadAssets(void)
{
	GD.safeload("assets.gd3");

	uint32_t freeAddress = ASSETS_END;

	freeAddress = fontLoad(FONT_SMALL, Nirmala14Header, "nirmala14.dat", freeAddress);
	freeAddress = fontLoad(FONT_LARGE, Nirmala18Header, "nirmala18.dat", freeAddress);
	freeAddress = fontLoad(FONT_LIGHT, Roboto14Header, "roboto14.dat", freeAddress);
	freeAddress = fontLoad(FONT_TITLE, Roboto20Header, "roboto20.dat", freeAddress);

	GD.loadptr = freeAddress;
}

void Button::draw(void)
{
	if (!visible)
		return;

	GD.Tag(tag);
	if (render != nullptr) {
		GD.LineWidth(16);
		if (!text.empty()) {
			((void (*)(const vec2&, bool, const LanguageString&))
				render)(xy, pressed | forcePressed, text);
		} else {
			((void (*)(const vec2&, bool))render)(xy,
				pressed | forcePressed);
		}
		GD.ColorRGB(WHITE);
	} else if (handle != -1) {
		GD.Vertex2ii(xy.x, xy.y, handle);
	}
	GD.Tag(0);
}

int Button::getTag(void) const
{
	return tag;
}

void Button::setForcePressed(bool p)
{
	forcePressed = p;
}

void Button::setVisibility(bool v)
{
	visible = v;
}

void Button::setText(const LanguageString& t)
{
	if (!text.empty())
		text = t;
}

bool Button::getPressed(void) const
{
	return pressed;
}

void Button::setPressed(bool p)
{
	pressed = p;
}

void Button::doAction(void)
{
	if (action != nullptr)
		action(pressed);
}

void Button::noDraw(const vec2& xy, bool pressed)
{
	(void)xy;
	(void)pressed;
}

void Button::drawDots(const vec2& xy, bool pressed)
{
	(void)xy;
	(void)pressed;

	GD.ColorRGB(NC_BKGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(0, 0);
	GD.Vertex2ii(57, 57);

	GD.ColorRGB(NC_FRGND_COLOR);
	GD.PointSize(20 * 4);
	GD.Begin(POINTS);
	GD.Vertex2ii(20, 30);
	GD.Vertex2ii(33, 30);
	GD.Vertex2ii(46, 30);
	GD.ColorRGB(NC_BKGND_COLOR);
	GD.PointSize(14 * 3);
	GD.Vertex2ii(20, 30);
	GD.Vertex2ii(33, 30);
	GD.Vertex2ii(46, 30);
}

void Button::drawBackArrow(const vec2& xy, bool pressed)
{
	(void)xy;
	(void)pressed;

	GD.ColorRGB(NC_BKGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(0, 0);
	GD.Vertex2ii(57, 57);

	GD.ColorRGB(NC_FRGND_COLOR);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(29, 23);
	GD.Vertex2ii(22, 30);
	GD.Vertex2ii(29, 37);
}

void Button::drawMenuItem(const vec2& xy, bool pressed, const LanguageString& text)
{
	(void)pressed;

	// Rectangle
	GD.Begin(RECTS);
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Vertex2ii(0, xy.y + 1);
	GD.Vertex2ii(272, xy.y + 59);
	GD.ColorRGB(WHITE);
	GD.Vertex2ii(0, xy.y);
	GD.Vertex2ii(272, xy.y);

	// Right arrow
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(272 - 29, xy.y + 23);
	GD.Vertex2ii(272 - 22, xy.y + 30);
	GD.Vertex2ii(272 - 29, xy.y + 37);

	// Text
	GD.cmd_text(30, xy.y + 30, FONT_SMALL, OPT_CENTERY, text());
}

void Button::drawScrollButton(const vec2& xy, bool pressed)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(0, xy.y + 1);
	GD.Vertex2ii(272, xy.y + 59);
	GD.ColorRGB(WHITE);
	GD.Vertex2ii(0, xy.y);
	GD.Vertex2ii(272, xy.y);

	// Arrow (x > 0 -> up, x < 0 -> down)
	GD.ColorRGB(WHITE);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	int a = (xy.x > 0) ? 40 : 20;
	int b = (xy.x > 0) ? 20 : 40;
	GD.Vertex2ii(136 - 30, xy.y + a);
	GD.Vertex2ii(136,      xy.y + b);
	GD.Vertex2ii(136 + 30, xy.y + a);
}

void Button::drawDispenserItem(const vec2& xy, bool pressed, const LanguageString& text)
{
	// Rectangle
	GD.Begin(RECTS);
	GD.ColorRGB(pressed ? 0x0c3d6b : NC_FRGND_COLOR);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 134, xy.y + 68);

	GD.ColorRGB(WHITE);
	GD.cmd_text(xy.x + 67, xy.y + 33, FONT_SMALL, OPT_CENTER, text(), 20);
}

void Button::drawFullWidth(const vec2& xy, bool pressed, const LanguageString& text)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(0, xy.y + 1);
	GD.Vertex2ii(272, xy.y + 59);
	GD.ColorRGB(WHITE);
	GD.Vertex2ii(0, xy.y);
	GD.Vertex2ii(272, xy.y);

	// Text
	GD.ColorRGB(WHITE);
	GD.cmd_text(136, xy.y + 30, FONT_SMALL, OPT_CENTER, text());
}

void Button::drawSmallLeft(const vec2& xy, bool pressed)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 40, xy.y + 40);

	// Left arrow
	GD.ColorRGB(WHITE);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(24, xy.y + 14);
	GD.Vertex2ii(17, xy.y + 21);
	GD.Vertex2ii(24, xy.y + 28);
}

void Button::drawSmallRight(const vec2& xy, bool pressed)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 40, xy.y + 40);

	// Right arrow
	GD.ColorRGB(WHITE);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(272 - 24, xy.y + 14);
	GD.Vertex2ii(272 - 17, xy.y + 21);
	GD.Vertex2ii(272 - 24, xy.y + 28);
}

void Button::drawUpButton(const vec2& xy, bool pressed)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 70, xy.y + 34);

	// Arrow
	GD.ColorRGB(WHITE);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(xy.x + 28, xy.y + 18);
	GD.Vertex2ii(xy.x + 35, xy.y + 11);
	GD.Vertex2ii(xy.x + 42, xy.y + 18);
}

void Button::drawDownButton(const vec2& xy, bool pressed)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 70, xy.y + 34);

	// Arrow
	GD.ColorRGB(WHITE);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(xy.x + 28, xy.y + 11);
	GD.Vertex2ii(xy.x + 35, xy.y + 18);
	GD.Vertex2ii(xy.x + 42, xy.y + 11);
}

void Button::drawExclusiveOption(const vec2& xy, bool pressed, const LanguageString& text)
{
	// Rectangle
	GD.ColorRGB(NC_FDGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 122, xy.y + 34);

	// Inner Rectangle
	GD.ColorRGB(pressed ? WHITE : NC_FRGND_COLOR);
	GD.Vertex2ii(xy.x + 1, xy.y + 1);
	GD.Vertex2ii(xy.x + 120, xy.y + 32);

	// Text
	GD.ColorRGB(pressed ? NC_FDGND_COLOR : WHITE);
	GD.cmd_text(xy.x + 62, xy.y + 17, FONT_SMALL, OPT_CENTER, text());
}

void Button::drawToggle(const vec2& xy, bool pressed)
{
	//if (pressed)
	//	extraState ^= true;

	GD.cmd_fgcolor(0xdddddd);
	GD.cmd_bgcolor(pressed ? NC_ONTOG_COLOR : NC_OFTOG_COLOR);
	GD.cmd_toggle(xy.x, xy.y, 30, 27, OPT_FLAT, pressed ? 65535 : 0,
		"\0\xFF\0");
}

void Screen::prepare(void)
{
	if (preShow != nullptr)
		preShow();
}

void Screen::show(void)
{
	//
	// Rendering stage
	//

	// Call the pre-render function
	if (preDraw != nullptr)
		preDraw();

	// Render the buttons
	for (unsigned int i = 0; i < buttonCount; i++)
		buttons[i].draw();

	// Push to the display
	GD.swap();

	//
	// Input stage
	//

	// Get display inputs
	GD.get_inputs();
	int tag = GD.inputs.tag;

	// If touched, reset the sleep countdown
	if (tag != 0)
		sleepCounter = 0;

	// Handle each button (must handle all every time to catch release
	// events)
	for (unsigned int i = 0; i < buttonCount; i++) {
		bool pressed = (buttons[i].getTag() == tag);
		bool change = (pressed != buttons[i].getPressed());
		buttons[i].setPressed(pressed);
		if (change) {
			buttons[i].doAction();
			break; // Only one change at a time
		}
	}

	// If no touch, continue sleep countdown
	if (tag == 0) {
		sleepCounter += 10;
		if (sleepCounter >= SLEEP_TIMEOUT) {
			// Timed out; enter sleep mode
			sleepCounter = 0;
			screenCurrent = &screenSleep;
		}
	}
}

void Screen::clearWithIonHeader(bool line)
{
	GD.ClearColorRGB(NC_BKGND_COLOR);
	GD.Clear();
		
	GD.Begin(BITMAPS);
	GD.ColorRGB(WHITE);
	GD.Vertex2ii(202, 17, IONLOGO_HANDLE);

	if (line) {
		GD.Begin(RECTS);
		GD.Vertex2ii(0, 60);
		GD.Vertex2ii(272, 60);
	}
}

