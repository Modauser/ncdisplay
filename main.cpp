/**
 * @file main.cpp
 * @brief Main entry point for system code.
 */

#include <atmel_start.h>
#include <hal_ext_irq.h>
#include <config/hpl_sercom_config.h>
#include <spi_lite.h>

#include <gameduino2/GD2.h>
constexpr uint32_t NC_BKGND_COLOR = 0xD6ECF9;
constexpr uint32_t NC_FRGND_COLOR = 0x026FB6;

// Enables USB drive detection and access.
// Comment to disable.
#define ALLOW_USB

// Constants used to verify and load the 'user' binary.
// NCIdentifier is a magic value that appears at the beginning of the binary.
// ncident is a pointer to where the binary's identifier is stored.
// ncmain is a function pointer to the 'user' binary entry point.
constexpr uint32_t NCIdentifier = 0x31415926;
static uint32_t *ncident = (uint32_t *)0x00040000;
static void (*ncmain)(void) = (void (*)(void))(0x00040004 | 1);

// FatFS objects for the SD card and USB.
static FATFS sdmmc_fatfs;
static FATFS msc_fatfs;

/**
 * Initializes the SD card and USB (if enabled).
 * Returns -1 if no SD card, 1 if no USB, or 0 if both are found.
 */
int initDisks(void);

// Include FatFS code; workaround to avoid compiling/linking issues.
#include "fatfs/diskio.c"

void esp32Request(void);

//
// Main entry point
//
int main(void)
{
	// Initializes MCU, drivers and middleware
	atmel_start_init();

	// Needed so USB interrupts can happen within service calls
	NVIC_SetPriority(SVCall_IRQn, 250);

	// Enable SPI
	SPI_0_enable();

    // Set up ESP32 listener
    ext_irq_register(PIN_PA14, esp32Request);

	// Initialize storage devices
	initDisks();

	// Confirm existance of display code, then jump to it
	if (*ncident == NCIdentifier)
		ncmain();

	while (1)
		delay_ms(250);
}

// Function prototypes for service call handler below
// Function documentation below
uint8_t spi_xfer_byte(uint8_t send);
int serialTest(void);
void firmware_update(uint32_t addr, uint32_t count, GDClass *gd);
uint32_t fatfs_svc_handler(uint32_t *fatfs_args);

/**
 * Handles service calls, which are made with the "svc" instruction.
 * 'User' code accesses hardware through making service calls.
 */
void SVCall_Handler(void)
{
	// Get call arguments from the proper stack (stored in msp or psp).
	uint32_t *args;
	asm("\
		tst lr, #4; \
		ite eq; \
		mrseq %0, msp; \
		mrsne %0, psp; \
	" : "=r" (args));

	// Get the service call number, then make the appropriate call.
	int svc_number = ((char *)args[6])[-2];

	switch (svc_number) {
	// "svc 0"
	// Millisecond delay.
	case 0:
		delay_ms(args[1]);
		break;
	// "svc 1"
	// Does a byte transfer over SPI.
	case 1:
		*((uint32_t *)args[0]) = spi_xfer_byte(args[1]);
		break;
	// "svc 2"
	// Does a storage device related call.
	case 2:
		*((uint32_t *)args[0]) = fatfs_svc_handler((uint32_t *)args[1]);
		break;
	// "svc 3"
	// Does a firmware update.
	case 3:
		firmware_update(args[0], args[1], (GDClass *)args[2]);
		break;
	// "svc 4"
	// Returns location of our printf function, which is connected to UART.
	case 4:
		*((uint32_t *)args[0]) = (uint32_t)printf;
		break;
	// "svc 5"
	// Reads bytes from UART, or does a serial test by checking for received
	// bytes.
	case 5:
		*((uint32_t *)args[0]) = (args[1] == 0x1234) ? serialTest() :
			getchar();
		break;
	default:
		break;
	}
}

uint8_t spi_xfer_byte(uint8_t send)
{
	return SPI_0_exchange_data(send);
}

int serialTest(void)
{
	// Wait to see if we receive a byte
	int i;
	for (i = 0; !_usart_sync_is_byte_received(&USART_0.device) && i < 12; i++)
		delay_ms(500);

	// Error on timeout or byte not being a '$'
	if (i == 12)
		return -1;
	if (int dollar = _usart_sync_read_byte(&USART_0.device); dollar != '$')
		return -1;

	// '$' is always followed by data; return that data
	return getchar();
}

void firmware_update(uint32_t addr, uint32_t count, GDClass *gd)
{
	if (*((uint32_t *)addr) != NCIdentifier)
		return;

	const uint32_t pageSize = flash_get_page_size(&FLASH_0);
	uint32_t writeCount = pageSize;
	uint32_t destAddr = 0x00040000;
	uint8_t *srcAddr = (uint8_t *)addr;
	while (count > 0) {
		if (count < pageSize)
			writeCount = count;

		flash_unlock(&FLASH_0, destAddr, 1);
		flash_write(&FLASH_0, destAddr, srcAddr, writeCount);
		flash_lock(&FLASH_0, destAddr, 1);

		destAddr += writeCount;
		srcAddr += writeCount;
		count -= writeCount;
	}

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

	GD.ClearColorRGB(NC_BKGND_COLOR);
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Clear();
	GD.cmd_text(136, 30, 23, OPT_CENTER, "USB Firmware Update");
	GD.cmd_text(10, 140, 18, 0, "Update applied.");
	GD.cmd_text(10, 170, 18, 0, "Please restart the machine.");
	GD.cmd_text(10, 230, 18, 0, "The power button is located");
	GD.cmd_text(10, 260, 18, 0, "on the back of the unit.");
	GD.swap();

	//NVIC_SystemReset();
	while (1);
}

uint32_t fatfs_svc_handler(uint32_t *fatfs_args)
{
	uint32_t ret = 0;

	switch (fatfs_args[0]) {
	case 0:
		// Opens a file
		ret = f_open((FIL *)fatfs_args[1], (const TCHAR *)fatfs_args[2],
			fatfs_args[3]);
		break;
	case 1:
		// Closes the file
		ret = f_close((FIL *)fatfs_args[1]);
		break;
	case 2:
		// Reads from an opened file
		ret = f_read((FIL *)fatfs_args[1], (void *)fatfs_args[2],
			fatfs_args[3], (UINT *)fatfs_args[4]);
		break;
	case 3:
		// Writes to an opened file
		ret = f_write((FIL *)fatfs_args[1], (const void *)fatfs_args[2],
			fatfs_args[3], (UINT *)fatfs_args[4]);
		break;
	case 4:
		// Deletes a file
		ret = f_unlink((const TCHAR *)fatfs_args[1]);
		break;
	case 5:
		// Reads in a line from an opened file
		ret = (uint32_t)f_gets((TCHAR *)fatfs_args[1], fatfs_args[2],
			(FIL *)fatfs_args[3]);
		break;
	case 6:
		// Opens a directory (for looking at directory entries)
		// Note: Opened directories do not need to be closed
		ret = (uint32_t)f_opendir((DIR *)fatfs_args[1],
			(const TCHAR *)	fatfs_args[2]);
		break;
	case 7:
		// Gets the next entry in the opened directory
		ret = (uint32_t)f_readdir((DIR *)fatfs_args[1],
			(FILINFO *)fatfs_args[2]);
		break;
	case 8:
		// Unmount a drive
		ret = (uint32_t)f_mount(nullptr, (const TCHAR *)fatfs_args[1],
			0);
		break;
	default:
		break;
	}

	return ret;
}

int initDisks(void)
{
	dstatus_t status;
	FRESULT result;
	int ret = 0;

	// Begin searching for a connected SD card
	status = STA_NOINIT;
	do {
		status = disk_status(DEV_SD);
		delay_ms(200);
	} while (status != 0 && status != STA_NODISK);

	if (status == STA_NODISK) {
		// No SD card found
		ret = -1;
		//return ret;
	} else {
		// SD card found; mount it
		result = f_mount(&sdmmc_fatfs, DRV_SD, 1);
		if (result != FR_OK) {
			ret = -1; // Maybe use USB instead, see above
			//return ret;
		}
	}

#ifdef ALLOW_USB
	// Search for a USB driver with a 3 second timeout
	status = STA_NOINIT;
	usbhc_start(&USB_HOST_CORE_INSTANCE_inst);
	int tries = 15;
	do {
		status = disk_status(DEV_USB);
		delay_ms(200);
	} while (status != 0 && status != STA_NODISK && (--tries > 0));

	if (tries <= 0)
		status = STA_NODISK;

	if (status == STA_NODISK) {
		// No USB found
		ret = 1;
	} else {
		// Fix: delay needed for serial to handshake properly...
		delay_ms(2000);

		// USB drive found; mount it
		result = f_mount(&msc_fatfs, DRV_USB, 1);
		//if (result != FR_OK)
		//	printf("USB drive mounting failed (%d).\r\n", result);
	}
#endif // ALLOW_USB

	(void)result;
	return ret;
}

void esp32Request(void)
{

}

void UsageFault_Handler(void)
{
	while (1);
}

void HardFault_Handler(void)
{
	while (1);
}

