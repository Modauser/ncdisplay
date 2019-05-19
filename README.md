# ncdisplay

To build ncdisplay, simply run `make` from the `gcc` directory. Ncdisplay consists of a core System part, and a user-interface User part. These parts can be built separately using `make system` or `make user`. The `-j` option is supported for parallel compilation. The resulting ELF files are `gcc/System.elf` and `gcc/User.elf`.

See the [wiki](https://github.com/tcsullivan/ncdisplay/wiki) for information on preparing a (Linux) computer for this project, as well as information on programming/updating files on the display. 

The System part of ncdisplay is based off of configuration files/libraries from [Atmel START](https://start.atmel.com).
The following additional libraries are used:
* FatFs, found [here](http://elm-chan.org/fsw/ff/00index_e.html)
* Gameduino, found [here](http://excamera.com/sphinx/gameduino2/code.html)
  
The ```nccode``` directory contains code specific to the display (User part), including the screens, main board communication, etc.

The ```sdcard_files``` directory contains a ZIP of the assets (image/font files) that should be on the microSD card.
