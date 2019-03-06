# ncdisplay

See [here](https://github.com/tcsullivan/ncdisplay/wiki/Preparation-on-Debian-testing) for how to prepare a computer for building ncdisplay. The guide is written for Debian testing, though it can be adapted to other Linux-based systems.

To build ncdisplay, simply run `make` from the `gcc` directory. Parts of ncdisplay can be built separately throgh `make system` or `make user`. The `-j` option is supported for parallel compilation. The resulting ELF files are `gcc/AtmelStart.elf` and `gcc/User.elf`.

See [here](https://github.com/tcsullivan/ncdisplay/wiki/Flashing-ncdisplay-with-OpenOCD) for instructions on flashing the display prototype. If ncdisplay has been flashed already, updates can optionally be given by putting `gcc/User.bin` on a USB drive as `update.bin`.

This project is based off of configuration files/libraries from [Atmel START](https://start.atmel.com).
The following additional libraries are used:
* FatFs, found [here](http://elm-chan.org/fsw/ff/00index_e.html)
* Gameduino, found [here](http://excamera.com/sphinx/gameduino2/code.html)
  
The ```nccode``` directory contains code specific to the display, including the screens, main board communication, etc.

The ```sdcard_files``` directory contains the assets (image/font files) that should be on the SD card.

