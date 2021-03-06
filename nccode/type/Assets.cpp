#include "Assets.h"
#include "NirmalaMod.h" // Contains font data 

#include <gameduino2/GD2.h>

uint32_t FREE_MEM = ASSETS_END;

void loadImage(int handle, const char *path, uint32_t addr)
{
	//uint32_t base, width, height;

	addr = (addr + 1023) & ~1023;

	GD.BitmapHandle(handle);
	GD.cmd_loadimage(addr, 0);
	GD.load(path);
	//GD.cmd_getprops(base, width, height);
	GD.finish();
	
	//imageNextAddress = base + width * height * 2;
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

	// Update header to have proper address of font data
	//*((uint32_t *)(header + 144)) = address + 148;

	// Prepare bitmap of font data
	GD.BitmapHandle(handle);
	uint32_t fileSize = GD.loadMem(datafile, address + 148);
	GD.cmd_setbitmap(address + 148, header[128], header[136], header[140]);

	// Write header to video RAM and enable the font
	GD.wr_n(address, (byte *)header, 148);
	GD.cmd_setfont(handle, address);
	return address + 148 + fileSize;
}

void loadAssets(void)
{
	GD.safeload("assets.gd3");

	uint32_t freeAddress = ASSETS_END;
	freeAddress = fontLoad(FONT_SMALL, NirmalaBold14Header, "nirmab14.dat", freeAddress);
	freeAddress = fontLoad(FONT_LARGE, NirmalaBold18Header, "nirmab18.dat", freeAddress);
	freeAddress = fontLoad(FONT_LIGHT, Nirmala14Header, "nirma14.dat", freeAddress);
	freeAddress = fontLoad(FONT_TITLE, Nirmala20Header, "nirma20.dat", freeAddress);

	FREE_MEM = freeAddress;
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

