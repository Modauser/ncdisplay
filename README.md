# ncdisplay

Building the code for ncdisplay requires:
* the latest arm-none-eabi toolchain (might be available for Windows from [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads))
* make
  
A Makefile to compile the program is available in the ```gcc``` directory. Simply run ```make``` to build both the system and user code, or run ```make system``` or ```make user``` to build only a single part. The ```-j``` option is supported for parallel compilation. The resulting system ELF file is ```gcc/AtmelStart.elf```, and the resulting user ELF file is ```gcc/User.elf```.
  
I use [OpenOCD](http://openocd.org/) to upload the ELFs to the device. [This](http://openocd.zylin.com/#/c/4272/) change/patch adds support for the SAME54 Xplained board. Any other program that's able to flash one of the ELF or HEX files will work as well.  
Flash User.elf on top of AtmelStart.elf (don't let an erase happen between flashes).  

This project is based off of configuration files/libraries from [Atmel START](https://start.atmel.com).
The following additional libraries are used:
* FatFs, found [here](http://elm-chan.org/fsw/ff/00index_e.html)
* Gameduino, found [here](http://excamera.com/sphinx/gameduino2/code.html)
  
The ```nccode``` directory contains code specific to the display, including the screens, main board communication, etc.

The ```sdcard_files``` directory contains the assets (image/font files) that should be on the SD card.

