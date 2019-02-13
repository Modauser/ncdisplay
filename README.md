# ncdisplay

Building the code for ncdisplay requires:
* arm-none-eabi toolchain (might be available from [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads))
* make
  
A Makefile to compile the program is available in the ```gcc``` directory. The display program has been split into two to allow for firmware updates; run ```make``` to build the core half and ```make user``` to build the "user" half (contains code for screens).  The ```-j``` option is supported for parallel compilation. The resulting core ELF file is ```gcc/AtmelStart.elf```, and the resulting user ELF file is ```gcc/User.elf```.
  
I use [OpenOCD](http://openocd.org/) to upload the ELFs to the device. [This](http://openocd.zylin.com/#/c/4272/) change/patch adds support for the SAME54 Xplained board. Any other program that's able to flash one of the ELF or HEX files will work as well.  
Flash User on top of AtmelStart without erasing the memory between writes.  

This project is based off of configuration files/libraries from [Atmel START](https://start.atmel.com).
The following additional libraries are used:
* FatFs, found [here](http://elm-chan.org/fsw/ff/00index_e.html)
* Gameduino, found [here](http://excamera.com/sphinx/gameduino2/code.html)
  
The ```nccode``` directory contains code specific to the display, including the screens, main board communication, etc. The file ```nccode/Assets.h``` includes a macro at the top of the file, USE_SERIAL, that's currently commented. To communicate with the main board and see all UI elements (like date/time), uncomment the line with USE_SERIAL.
