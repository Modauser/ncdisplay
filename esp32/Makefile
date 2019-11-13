CROSS = esp32/tools/xtensa-esp32-elf/bin/xtensa-esp32-elf-
CXX = $(CROSS)g++
CC = $(CROSS)gcc

PORT = COM3

ARDUINO_LIB_WIFI = esp32/libraries/WiFi/src
ARDUINO_LIB_WIFI_SRC = $(ARDUINO_LIB_WIFI)/ETH.cpp \
					   $(ARDUINO_LIB_WIFI)/WiFi.cpp \
					   $(ARDUINO_LIB_WIFI)/WiFiAP.cpp \
					   $(ARDUINO_LIB_WIFI)/WiFiClient.cpp \
					   $(ARDUINO_LIB_WIFI)/WiFiGeneric.cpp \
					   $(ARDUINO_LIB_WIFI)/WiFiMulti.cpp \
					   $(ARDUINO_LIB_WIFI)/WiFiSTA.cpp \
					   $(ARDUINO_LIB_WIFI)/WiFiScan.cpp \
					   $(ARDUINO_LIB_WIFI)/WiFiServer.cpp \
					   $(ARDUINO_LIB_WIFI)/WiFiUdp.cpp
ARDUINO_CORE_ESP32 = esp32/cores/esp32
ARDUINO_CORE_ESP32_SRC = $(ARDUINO_CORE_ESP32)/esp32-hal-adc.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-bt.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-cpu.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-dac.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-gpio.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-i2c.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-ledc.c	\
						 $(ARDUINO_CORE_ESP32)/esp32-hal-matrix.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-misc.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-psram.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-rmt.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-sigmadelta.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-spi.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-time.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-timer.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-touch.c \
						 $(ARDUINO_CORE_ESP32)/esp32-hal-uart.c \
						 $(ARDUINO_CORE_ESP32)/stdlib_noniso.c \
						 $(ARDUINO_CORE_ESP32)/wiring_pulse.c \
						 $(ARDUINO_CORE_ESP32)/wiring_shift.c \
						 $(ARDUINO_CORE_ESP32)/libb64/cdecode.c \
						 $(ARDUINO_CORE_ESP32)/libb64/cencode.c \
						 $(ARDUINO_CORE_ESP32)/Esp.cpp \
						 $(ARDUINO_CORE_ESP32)/FunctionalInterrupt.cpp \
						 $(ARDUINO_CORE_ESP32)/HardwareSerial.cpp \
						 $(ARDUINO_CORE_ESP32)/IPAddress.cpp \
						 $(ARDUINO_CORE_ESP32)/IPv6Address.cpp \
						 $(ARDUINO_CORE_ESP32)/MD5Builder.cpp \
						 $(ARDUINO_CORE_ESP32)/Print.cpp \
						 $(ARDUINO_CORE_ESP32)/Stream.cpp \
						 $(ARDUINO_CORE_ESP32)/StreamString.cpp \
						 $(ARDUINO_CORE_ESP32)/WMath.cpp \
						 $(ARDUINO_CORE_ESP32)/WString.cpp \
						 $(ARDUINO_CORE_ESP32)/base64.cpp \
						 $(ARDUINO_CORE_ESP32)/cbuf.cpp \
						 $(ARDUINO_CORE_ESP32)/main.cpp
SRC = $(ARDUINO_LIB_WIFI_SRC) \
	  $(ARDUINO_CORE_ESP32_SRC) \
	  esp32.cpp

OBJ = $(SRC:=.o)
ELF = esp32.elf
BIN = esp32.bin

CINC = $(foreach d, $(wildcard esp32/tools/sdk/include/*), -I$d) \
		 -I$(ARDUINO_CORE_ESP32) -I$(ARDUINO_LIB_WIFI) \
		 "-Iesp32/variants/esp32" \

CDEF = -DESP32 -DESP_PLATFORM -DARDUINO_ESP32_DEV -DARDUINO_ARCH_ESP32 \
	   -DARDUINO_BOARD="ESP32_DEV" -DARDUINO_VARIANT="esp32" \
	   -DF_CPU=80000000L -DARDUINO=10805 \
	   -DHAVE_CONFIG_H -DGCC_NOT_5_2_0=0 -DWITH_POSIX -DCORE_DEBUG_LEVEL=0 \
	   -DMBEDTLS_CONFIG_FILE="mbedtls/esp_config.h"

CXXFLAGS = -std=gnu++11 -Os -g3 -c \
		   $(CDEF) $(CINC) \
		   -fexceptions -fno-rtti -fstack-protector -fstrict-volatile-bitfields \
		   -ffunction-sections -fdata-sections \
		   -mlongcalls -nostdlib \
		   -Wpointer-arith \
		   -Wno-error=maybe-uninitialized -Wno-error=unused-function -Wno-error=unused-but-set-variable \
		   -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wno-unused-parameter -Wno-unused-but-set-parameter \
		   -Wno-missing-field-initializers -Wno-sign-compare

CFLAGS = -std=gnu99 -Os -g3 -c \
		 $(CDEF) $(CINC) \
		 -mlongcalls -nostdlib \
		 -Wall -Wextra

LIB = $(wildcard esp32/tools/sdk/lib/*)
LINK = -Lesp32/tools/sdk/ld \
	   -T esp32_out.ld -T esp32.common.ld -T esp32.rom.ld \
	   -T esp32.peripherals.ld -T esp32.rom.libgcc.ld \
	   -T esp32.rom.spiram_incompatible_fns.ld \
	   -u ld_include_panic_highint_hdl -u call_user_start_cpu0 \
	   -u __cxa_guard_dummy -u __cxx_fatal_exception \
	   -Wl,--gc-sections -Wl,-static -Wl,--undefined=uxTopUsedPriority

all: $(ELF) $(BIN)

clean:
	@echo "  CLEAN"
	@rm -f $(OBJ) $(ELF) $(BIN)

upload:
	@echo "  UPLOAD"
	@python esp32/tools/esptool/esptool.py --port $(PORT) --baud 115200 \
		--before default_reset --after hard_reset \
		write_flash --flash_freq 80m --flash_mode dio --flash_size detect \
		--compress \
		0x0e000 esp32/tools/partitions/boot_app0.bin \
		0x01000 esp32/tools/sdk/bin/bootloader_qio_80m.bin \
		0x10000 esp32.bin \
		0x08000 esp32.partitions.bin

$(ELF): $(OBJ)
	@echo "  CC    " $(ELF)
	@$(CC) -nostdlib $(LINK) \
		-Wl,--start-group $(OBJ) $(LIB) -lgcc -lstdc++ -Wl,--end-group \
		-Wl,-EL -o $(ELF)

$(BIN): $(ELF)
	@python esp32/tools/gen_esp32part.py -q esp32/tools/partitions/default.csv \
		esp32.partitions.bin
	@python esp32/tools/esptool/esptool.py --chip esp32 elf2image \
		--flash_mode "dio" --flash_freq "80m" --flash_size "4MB" -o $(BIN) $(ELF)

%.cpp.o: %.cpp
	@echo "  CXX   " $<
	@$(CXX) $(CXXFLAGS) $< -o $@

%.c.o: %.c
	@echo "  CC    " $<
	@$(CC) $(CFLAGS) $< -o $@

