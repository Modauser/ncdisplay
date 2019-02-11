ifdef SystemRoot
	SHELL = cmd.exe
	MK_DIR = mkdir
else
	ifeq ($(shell uname), Linux)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), CYGWIN)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW32)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW64)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
hpl/ramecc \
hpl/nvmctrl \
sd_mmc \
hpl/systick \
hpl/oscctrl \
hpl/sdhc \
stdio_redirect \
gcc \
hpl/osc32kctrl \
usb \
hpl/dmac \
usb/class/msc/host \
hal/src \
stdio_redirect/gcc \
hal/utils/src \
diskio \
hpl/usb \
hpl/pm \
hpl/cmcc \
hpl/gclk \
hpl/spi \
gcc/gcc \
hpl/mclk \
usb/host \
hpl/sercom \
hpl/core \
fatfs \
gameduino2 \
nccode

# List the object files
OBJS +=  \
hal/src/hal_io.o \
stdio_redirect/stdio_io.o \
hpl/systick/hpl_systick.o \
usb/class/msc/host/mschf.o \
stdio_redirect/gcc/write.o \
sd_mmc_start.o \
hpl/core/hpl_core_m4.o \
hal/src/hal_cache.o \
hal/src/hal_flash.o \
hpl/nvmctrl/hpl_nvmctrl.o \
hal/utils/src/utils_syscalls.o \
stdio_redirect/gcc/read.o \
sd_mmc/sd_mmc.o \
hpl/spi/spi_lite.o \
hal/src/hal_mci_sync.o \
hpl/usb/hpl_usb.o \
diskio/msc_diskio.o \
hal/src/hal_delay.o \
hpl/pm/hpl_pm.o \
hpl/core/hpl_init.o \
hpl/gclk/hpl_gclk.o \
hal/utils/src/utils_list.o \
hal/utils/src/utils_assert.o \
usb_start.o \
hpl/sdhc/hpl_sdhc.o \
hal/src/hal_usart_sync.o \
usb/host/usbhc.o \
hpl/mclk/hpl_mclk.o \
hpl/ramecc/hpl_ramecc.o \
stdio_start.o \
usb/usb_protocol.o \
hal/src/hal_init.o \
diskio_start.o \
gcc/gcc/startup_same54.o \
main.o \
hpl/osc32kctrl/hpl_osc32kctrl.o \
diskio/sdmmc_diskio.o \
hpl/oscctrl/hpl_oscctrl.o \
driver_init.o \
hpl/sercom/hpl_sercom.o \
hal/src/hal_atomic.o \
hal/src/hal_gpio.o \
hal/utils/src/utils_event.o \
hal/src/hal_sleep.o \
hpl/dmac/hpl_dmac.o \
gcc/system_same54.o \
atmel_start.o \
hpl/cmcc/hpl_cmcc.o \
fatfs/diskio.o \
fatfs/ff.o \
fatfs/ffsystem.o \
fatfs/ffunicode.o \
gameduino2/GD2.o \
nccode/Assets.o \
nccode/Dispense.o \
nccode/SettingsMenu.o \
nccode/Language.o \
nccode/Advanced.o \
nccode/ProductInfo.o \
nccode/Temperature.o \
nccode/Filter.o \
nccode/TimeDate.o \
nccode/SleepMode.o \
nccode/USBUpload.o \
nccode/ServiceDetails.o \
nccode/Lock.o \
nccode/Lockscreen.o \
nccode/SystemOptions.o \
nccode/LanguageString.o \
nccode/Sleep.o \
nccode/FilterChange.o \
nccode/FilterChangeTimer.o \
nccode/USBUpdate.o \
nccode/Warning.o \
nccode/Errors.o \
nccode/Init.o

OBJS_AS_ARGS +=  \
"hal/src/hal_io.o" \
"stdio_redirect/stdio_io.o" \
"hpl/systick/hpl_systick.o" \
"usb/class/msc/host/mschf.o" \
"stdio_redirect/gcc/write.o" \
"sd_mmc_start.o" \
"hpl/core/hpl_core_m4.o" \
"hal/src/hal_cache.o" \
"hal/src/hal_flash.o" \
"hpl/nvmctrl/hpl_nvmctrl.o" \
"hal/utils/src/utils_syscalls.o" \
"stdio_redirect/gcc/read.o" \
"sd_mmc/sd_mmc.o" \
"hpl/spi/spi_lite.o" \
"hal/src/hal_mci_sync.o" \
"hpl/usb/hpl_usb.o" \
"diskio/msc_diskio.o" \
"hal/src/hal_delay.o" \
"hpl/pm/hpl_pm.o" \
"hpl/core/hpl_init.o" \
"hpl/gclk/hpl_gclk.o" \
"hal/utils/src/utils_list.o" \
"hal/utils/src/utils_assert.o" \
"usb_start.o" \
"hpl/sdhc/hpl_sdhc.o" \
"hal/src/hal_usart_sync.o" \
"usb/host/usbhc.o" \
"hpl/mclk/hpl_mclk.o" \
"hpl/ramecc/hpl_ramecc.o" \
"stdio_start.o" \
"usb/usb_protocol.o" \
"hal/src/hal_init.o" \
"diskio_start.o" \
"gcc/gcc/startup_same54.o" \
"main.o" \
"hpl/osc32kctrl/hpl_osc32kctrl.o" \
"diskio/sdmmc_diskio.o" \
"hpl/oscctrl/hpl_oscctrl.o" \
"driver_init.o" \
"hpl/sercom/hpl_sercom.o" \
"hal/src/hal_atomic.o" \
"hal/src/hal_gpio.o" \
"hal/utils/src/utils_event.o" \
"hal/src/hal_sleep.o" \
"hpl/dmac/hpl_dmac.o" \
"gcc/system_same54.o" \
"atmel_start.o" \
"hpl/cmcc/hpl_cmcc.o" \
"fatfs/diskio.o" \
"fatfs/ff.o" \
"fatfs/ffsystem.o" \
"fatfs/ffunicode.o" \
"gameduino2/GD2.o" \
"nccode/Assets.o" \
"nccode/Dispense.o" \
"nccode/SettingsMenu.o" \
"nccode/Language.o" \
"nccode/Advanced.o" \
"nccode/ProductInfo.o" \
"nccode/Temperature.o" \
"nccode/Filter.o" \
"nccode/TimeDate.o" \
"nccode/SleepMode.o" \
"nccode/USBUpload.o" \
"nccode/ServiceDetails.o" \
"nccode/Lock.o" \
"nccode/Lockscreen.o" \
"nccode/SystemOptions.o" \
"nccode/LanguageString.o" \
"nccode/Sleep.o" \
"nccode/FilterChange.o" \
"nccode/FilterChangeTimer.o" \
"nccode/USBUpdate.o" \
"nccode/Warning.o" \
"nccode/Errors.o" \
"nccode/Init.o"

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

DEPS_AS_ARGS +=  \
"usb/host/usbhc.d" \
"stdio_redirect/stdio_io.d" \
"hal/utils/src/utils_event.d" \
"hal/src/hal_io.d" \
"hpl/ramecc/hpl_ramecc.d" \
"hpl/systick/hpl_systick.d" \
"hpl/core/hpl_core_m4.d" \
"hal/utils/src/utils_syscalls.d" \
"usb/class/msc/host/mschf.d" \
"sd_mmc_start.d" \
"stdio_redirect/gcc/write.d" \
"gcc/gcc/startup_same54.d" \
"hpl/usb/hpl_usb.d" \
"diskio/msc_diskio.d" \
"sd_mmc/sd_mmc.d" \
"hal/src/hal_mci_sync.d" \
"hpl/spi/spi_lite.d" \
"hal/utils/src/utils_list.d" \
"hpl/cmcc/hpl_cmcc.d" \
"usb_start.d" \
"hal/utils/src/utils_assert.d" \
"hpl/sdhc/hpl_sdhc.d" \
"hal/src/hal_flash.d" \
"hpl/nvmctrl/hpl_nvmctrl.d" \
"hal/src/hal_delay.d" \
"hpl/core/hpl_init.d" \
"hpl/pm/hpl_pm.d" \
"usb/usb_protocol.d" \
"stdio_start.d" \
"hpl/gclk/hpl_gclk.d" \
"hpl/dmac/hpl_dmac.d" \
"diskio_start.d" \
"hal/src/hal_init.d" \
"hal/src/hal_usart_sync.d" \
"hpl/mclk/hpl_mclk.d" \
"driver_init.d" \
"stdio_redirect/gcc/read.d" \
"hpl/osc32kctrl/hpl_osc32kctrl.d" \
"main.d" \
"diskio/sdmmc_diskio.d" \
"hal/src/hal_cache.d" \
"hal/src/hal_sleep.d" \
"hpl/sercom/hpl_sercom.d" \
"hal/src/hal_gpio.d" \
"hal/src/hal_atomic.d" \
"hpl/oscctrl/hpl_oscctrl.d" \
"gcc/system_same54.d" \
"atmel_start.d" \
"fatfs/diskio.d" \
"fatfs/ff.d" \
"fatfs/ffsystem.d" \
"fatfs/ffunicode.d" \
"gameduino2/GD2.d" \
"nccode/Assets.d" \
"nccode/Dispense.d" \
"nccode/SettingsMenu.d" \
"nccode/Language.d" \
"nccode/Advanced.d" \
"nccode/ProductInfo.d" \
"nccode/Temperature.d" \
"nccode/Filter.d" \
"nccode/TimeDate.d" \
"nccode/SleepMode.d" \
"nccode/USBUpload.d" \
"nccode/ServiceDetails.d" \
"nccode/Lock.d" \
"nccode/Lockscreen.d" \
"nccode/SystemOptions.d" \
"nccode/LanguageString.d" \
"nccode/Sleep.d" \
"nccode/FilterChange.d" \
"nccode/FilterChangeTimer.d" \
"nccode/USBUpdate.d" \
"nccode/Warning.d" \
"nccode/Errors.d" \
"nccode/Init.d"

CROSS = arm-none-eabi-
CC = gcc
CXX = g++
AS = as
OBJCOPY = objcopy

CINCLUDES = \
	-I"../" -I"../config" \
	-I"../hal/include" -I"../hal/utils/include" \
	-I"../hpl/cmcc" -I"../hpl/spi" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/mclk" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/ramecc" -I"../hpl/sdhc" -I"../hpl/sercom" -I"../hpl/systick" -I"../hpl/usb" \
	-I"../hri" \
	-I"../usb" -I"../usb/class/hub" -I"../usb/class/msc" -I"../usb/class/msc/host" -I"../usb/host" \
	-I"../diskio" -I"../stdio_redirect" -I"../sd_mmc" \
       	-I"../CMSIS/Include" -I"../include"

MCUFLAGS = -D__SAME54P20A__ -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mlong-calls
CXXFLAGS = -std=c++17 -fno-rtti -fno-exceptions -ffunction-sections
CFLAGS = -std=gnu99 -ffunction-sections
DFLAGS = -DDEBUG -ggdb -g3 -Wall

OUTPUT_FILE_NAME := AtmelStart
OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf

vpath %.cpp ../
vpath %.c ../
vpath %.s ../
vpath %.S ../

# All Target
all: $(SUB_DIRS) $(OUTPUT_FILE_PATH)

# Linker target

$(OUTPUT_FILE_PATH): $(OBJS)
	@echo $@
	@$(CROSS)$(CC) -o $(OUTPUT_FILE_NAME).elf $(OBJS_AS_ARGS) -DSTACK_SIZE=4096 \
-Wl,--start-group -lm -Wl,--end-group -mthumb \
-Wl,-Map="$(OUTPUT_FILE_NAME).map" --specs=nano.specs -Wl,--gc-sections -mcpu=cortex-m4 \
 \
-T"../gcc/gcc/same54p20a_flash.ld" \
-L"../gcc/gcc"

	@echo Finished building target: $@
	@$(CROSS)$(OBJCOPY) -O binary "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).bin"
	@$(CROSS)$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature  \
        "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).hex"
	@$(CROSS)$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma \
        .eeprom=0 --no-change-warnings -O binary "$(OUTPUT_FILE_NAME).elf" \
        "$(OUTPUT_FILE_NAME).eep" || exit 0
	@$(CROSS)objdump -h -S "$(OUTPUT_FILE_NAME).elf" > "$(OUTPUT_FILE_NAME).lss"
	@$(CROSS)size "$(OUTPUT_FILE_NAME).elf"

# Compiler targets

%.o: %.cpp
	@echo $<
	@$(CROSS)$(CXX) $(MCUFLAGS) $(CXXFLAGS) $(DFLAGS) $(CINCLUDES) -c \
	-Wextra -Wno-unused-variable -Wno-unused-parameter \
	-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

%.o: %.c
	@echo $<
	@$(CROSS)$(CC) $(MCUFLAGS) $(CFLAGS) $(DFLAGS) $(CINCLUDES) -c \
	-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

%.o: %.s
	@echo $<
	@$(CROSS)$(AS) -x c $(MCUFLAGS) $(CFLAGS) $(DFLAGS) $(CINCLUDES) -c -Os \
	-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

%.o: %.S
	@echo $<
	@$(CROSS)$(CC) -x c $(MCUFLAGS) $(CFLAGS) $(DFLAGS) $(CINCLUDES) -c -Os \
	-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean:
	@echo "Cleaning..."
	@rm -f $(OBJS_AS_ARGS)
	@rm -f $(OUTPUT_FILE_PATH)
	@rm -f $(DEPS_AS_ARGS)
	@rm -f $(OUTPUT_FILE_NAME).a $(OUTPUT_FILE_NAME).hex $(OUTPUT_FILE_NAME).bin \
        @$(OUTPUT_FILE_NAME).lss $(OUTPUT_FILE_NAME).eep $(OUTPUT_FILE_NAME).map \
        @$(OUTPUT_FILE_NAME).srec
