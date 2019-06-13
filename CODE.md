## Code guide

As mentioned in the README, there are two parts to ncdisplay: System and User.

The System part takes care of device hardware initialization and interaction.
Most of the files in this root directory belong to the System part; however,
the majority of these are simply generated files from Atmel. The most important
file here is `main.cpp`, which contains the program's main function/starting
point.

The User part is where all the display screens and main board requests are. Code
files and a CODE.md for this part are in the `nccode` folder.

---
Other files in this directory:
* `config/`: Contains processor configuration files, for things including clock
speeds and peripherial settings.
* `fatfs/`: The FatFS filesystem library. `fatfs/ffconf.h` is the configuration
file for this library.
* `gameduino2/`: The Gameduino library, used for drawing to the display.
* `gcc/`: Files for code compilation, including the Makefile.
* `sdcard_files/`: Contains a zip of the files that need to be on the display's
microSD card.
* `simulator/`: A WIP Linux program for simulating a main board connection.
* `installUdevRules.sh`: Adds `98-openocd.rules` to udev so a non-root user can
access the programmer device.

