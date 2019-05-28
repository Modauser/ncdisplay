/**
 * @file Preinit.cpp
 * @brief Landing point from main firmware. Does variable
 * initialization/construction.
 */

#include "SystemCalls.h"

#include <cstdarg>
#include <cstdint>

// Linker variables
extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));
extern void (*__fini_array_start []) (void) __attribute__((weak));
extern void (*__fini_array_end []) (void) __attribute__((weak));

// More linker variables
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;
//extern void __libc_init_array(void);

// Pointer to system printf, for UART communication
int (*serialPrintf)(const char *, ...) = 0;

// Defined in Init.cpp
extern void setup(void);
extern void loop(void);

// Identifier/magic number. Placed at start of binary for system validation.
__attribute__((section(".ncident")))
static volatile uint32_t NCIdentifier = 0x31415926;

// 'User' binary entry point. Placed after identifier.
__attribute__((section(".ncstart")))
__attribute__((naked))
int main(void)
{
	// Load our stack
	asm("\
		mov r0, %0; \
		msr psp, r0; \
		mrs r0, control; \
		orr r0, r0, #2; \
		msr control, r0; \
	" :: "r" (&_estack));

	// Load variable data into memory
	uint32_t *pSrc, *pDest;
	pSrc = &_etext;
	pDest = &_srelocate;
	if (pSrc != pDest) {
		while (pDest < &_erelocate)
			*pDest++ = *pSrc++;
	}

	// Fill uninitialized variable data with zeros
	for (pDest = &_szero; pDest < &_ezero;)
		*pDest++ = 0;

	// Below is code that initializes variable data
	// __libc_init_array();
	uint32_t count, i;

	count = __preinit_array_end - __preinit_array_start;
	for (i = 0; i < count; i++)
		__preinit_array_start[i]();

	count = __init_array_end - __init_array_start;
	for (i = 0; i < count; i++)
		__init_array_start[i]();
	// end __libc_init_array();

	// Get printf for UART comm.
	serialPrintf = (int(*)(const char *, ...))getPrintf();

	// Call Init.cpp's setup()
	setup();

	// Loop on Init.cpp's loop()
	while (1)
		loop();
}

