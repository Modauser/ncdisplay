/**
 * @file Assets.cpp
 * @brief Functions for loading visual assets, and for other miscellaneous
 * things.
 */
#include "Assets.h"
#include "NirmalaMod.h" // Contains font data 

#include <gameduino2/GD2.h>

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

	GD.BitmapHandle(SETSTRIP_HANDLE);
	GD.BitmapLayout(ARGB4, 26 * 2, 26);
	GD.BitmapSize(NEAREST, BORDER, BORDER, 26, 26);

	GD.BitmapHandle(FLTSTRIP_HANDLE);
	GD.BitmapLayout(L8, 69, 68);
	GD.BitmapSize(NEAREST, BORDER, BORDER, 69, 68);

	GD.BitmapHandle(DROPSTRP_HANDLE);
	GD.BitmapLayout(ARGB2, 107, 133);
	GD.BitmapSize(NEAREST, BORDER, BORDER, 107, 133);

	uint32_t freeAddress = ASSETS_END;

	freeAddress = fontLoad(FONT_SMALL, Nirmala14Header, "nirmala14.dat", freeAddress);
	freeAddress = fontLoad(FONT_LARGE, Nirmala18Header, "nirmala18.dat", freeAddress);
	freeAddress = fontLoad(FONT_LIGHT, Roboto14Header, "roboto14.dat", freeAddress);
	freeAddress = fontLoad(FONT_TITLE, Roboto20Header, "roboto20.dat", freeAddress);

	GD.loadptr = freeAddress;
}

void clearScreenWithIonHeader(bool line)
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

