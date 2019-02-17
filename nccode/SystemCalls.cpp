#include <fatfs/ff.h>
#include <diskio.h>
#include <stdint.h>

uint32_t getPrintf(void)
{
	uint32_t addr = 0;
	asm("\
		mov r0, %0; \
		svc 4; \
	" :: "r" (&addr));
	return addr;
}

void firmwareUpdate(uint32_t addr, uint32_t count)
{
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 3; \
	" :: "r" (addr), "r" (count));
}

uint8_t spi_xfer_byte(uint8_t send)
{
	uint8_t recv = 0;
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 1; \
	" :: "r" (&recv), "r" (send));
	return recv;
}

FRESULT f_open(FIL *fp, const TCHAR *path, BYTE mode)
{
	FRESULT result = FR_OK;
	uint32_t args[4] = { 0, (uint32_t)fp, (uint32_t)path, mode };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

FRESULT f_close(FIL *fp)
{
	FRESULT result = FR_OK;
	uint32_t args[2] = { 1, (uint32_t)fp };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

FRESULT f_read(FIL *fp, void *buff, UINT btr, UINT *br)
{
	FRESULT result = FR_OK;
	uint32_t args[5] = { 2, (uint32_t)fp, (uint32_t)buff, btr, (uint32_t)br };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

FRESULT f_write(FIL *fp, const void *buff, UINT btw, UINT *bw)
{
	FRESULT result = FR_OK;
	uint32_t args[5] = { 3, (uint32_t)fp, (uint32_t)buff, btw, (uint32_t)bw };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

FRESULT f_unlink(const TCHAR *path)
{
	FRESULT result = FR_OK;
	uint32_t args[2] = { 4, (uint32_t)path };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (&result), "r" (args));
	return result;
}

TCHAR* f_gets(TCHAR *buff, int len, FIL *fp)
{
	TCHAR *result = 0;
	uint32_t args[4] = { 5, (uint32_t)buff, (uint32_t)len, (uint32_t)fp };
	asm("\
		mov r0, %0; \
		mov r1, %1; \
		svc 2; \
	" :: "r" (result), "r" (args));
	return result;
}

extern "C" {

void delay_ms(uint32_t ms)
{
	asm("\
		mov r1, %0; \
		svc 0; \
	" :: "r" (ms));
}

} // extern "C"


