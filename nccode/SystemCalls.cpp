// See SystemCalls.h for descriptions of what these functions do (with the
// exception of the FatFS functions)
//
// The SVC calls these functions make jumps to code in main.cpp
// See SVCall_Handler() in main.cpp for more info on SVC calls
//
// NOOPTIMIZE tags are required on these functions to prevent the compiler from
// messing up what we want.

#include "type/Assets.h"

#include <fatfs/ff.h>
#include <diskio.h>
#include <stdint.h>

NOOPTIMIZE
uint32_t getPrintf(void)
{
	volatile uint32_t addr = 0;
	asm("\
		mov r0, %0; \
		svc 4; \
	" :: "r" (&addr));
	return addr;
}

NOOPTIMIZE
int serialTest(void)
{
	volatile int ret = 0;
	asm("\
		mov r0, %0; \
		mov r1, 0x1234; \
		svc 5; \
	" :: "r" (&ret));
	return ret;
}

NOOPTIMIZE
int serialGetchar(void)
{
	volatile uint32_t ret = 0;
	asm("\
		mov r0, %0; \
		svc 5; \
	" :: "r" (&ret));
	return ret;
}

NOOPTIMIZE
uint8_t spi_xfer_byte(uint8_t send)
{
	volatile uint8_t recv = send;
	asm volatile("\
		mov r0, %0; \
		mov r1, %1; \
		svc 1; \
	" :: "r" (&recv), "r" (recv));
	return recv;
}

NOOPTIMIZE
void firmwareUpdate(uint32_t addr, uint32_t count)
{
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 3; \
	" :: "r" (addr), "r" (count));
}

NOOPTIMIZE
FRESULT f_open(FIL *fp, const TCHAR *path, BYTE mode)
{
	volatile FRESULT result = FR_OK;
	uint32_t args[4] = { 0, (uint32_t)fp, (uint32_t)path, mode };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

NOOPTIMIZE
FRESULT f_close(FIL *fp)
{
	volatile FRESULT result = FR_OK;
	uint32_t args[2] = { 1, (uint32_t)fp };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

NOOPTIMIZE
FRESULT f_read(FIL *fp, void *buff, UINT btr, UINT *br)
{
	volatile FRESULT result = FR_OK;
	uint32_t args[5] = { 2, (uint32_t)fp, (uint32_t)buff, btr, (uint32_t)br };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

NOOPTIMIZE
FRESULT f_write(FIL *fp, const void *buff, UINT btw, UINT *bw)
{
	volatile FRESULT result = FR_OK;
	uint32_t args[5] = { 3, (uint32_t)fp, (uint32_t)buff, btw, (uint32_t)bw };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

NOOPTIMIZE
FRESULT f_unlink(const TCHAR *path)
{
	volatile FRESULT result = FR_OK;
	uint32_t args[2] = { 4, (uint32_t)path };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

NOOPTIMIZE
TCHAR* f_gets(TCHAR *buff, int len, FIL *fp)
{
	volatile TCHAR *result = 0;
	uint32_t args[4] = { 5, (uint32_t)buff, (uint32_t)len, (uint32_t)fp };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (result), "r" (args));
	return (TCHAR *)result;
}

NOOPTIMIZE
FRESULT f_opendir(DIR *dir, const TCHAR *path)
{
	volatile FRESULT result = FR_OK;
	uint32_t args[3] = { 6, (uint32_t)dir, (uint32_t)path };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (result), "r" (args));
	return result;
}

NOOPTIMIZE
FRESULT f_readdir(DIR *dir, FILINFO *fileinfo)
{
	volatile FRESULT result = FR_OK;
	uint32_t args[3] = { 7, (uint32_t)dir, (uint32_t)fileinfo };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (result), "r" (args));
	return result;
}

// This is necessary, maybe because delay_ms header definition is also extern C'd
extern "C" {

NOOPTIMIZE
void delay_ms(uint32_t ms)
{
	asm("\
		mov r1, %0; \
		svc 0; \
	" :: "r" (ms));
}

} // extern "C"


