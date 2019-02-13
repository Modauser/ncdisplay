#include <cstdarg>
#include <cstdint>

extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));
extern void (*__fini_array_start []) (void) __attribute__((weak));
extern void (*__fini_array_end []) (void) __attribute__((weak));

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

extern uint32_t getPrintf(void);

int (*serialPrintf)(const char *, ...) = 0;

extern void setup(void);
extern void loop(void);

__attribute__((section(".ncident")))
static const uint32_t NCIdentifier = 0x31415926;

__attribute__((section(".ncstart")))
__attribute__((naked))
int main(void)
{
	asm("\
		mov r0, %0; \
		msr psp, r0; \
		mrs r0, control; \
		orr r0, r0, #2; \
		msr control, r0; \
	" :: "r" (&_estack));

	uint32_t *pSrc, *pDest;

	pSrc = &_etext;
	pDest = &_srelocate;
	if (pSrc != pDest) {
		while (pDest < &_erelocate)
			*pDest++ = *pSrc++;
	}

	for (pDest = &_szero; pDest < &_ezero;)
		*pDest++ = 0;

	//__libc_init_array();
	uint32_t count, i;
        
	count = __preinit_array_end - __preinit_array_start;
	for (i = 0; i < count; i++)
		__preinit_array_start[i]();
	
	count = __init_array_end - __init_array_start;
	for (i = 0; i < count; i++)
		__init_array_start[i]();
	// end __libc_init_array();

	serialPrintf = (int(*)(const char *, ...))getPrintf();

	setup();

	while (1)
		loop();
}

