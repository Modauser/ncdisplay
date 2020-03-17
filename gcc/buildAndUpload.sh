#!/bin/bash
#
# A simple script to automate compilation and uploading.
# Written by Clyne.
#

MAKEARGS = -j4



# Build program
echo
echo "Building code: 'make'..."
echo
make system user $MAKEARGS
if [ $? -ne 0 ]
then
	echo
	echo "Building code failed."
	exit 1
fi

pause
# Start OpenOCD
echo
echo "Starting OpenOCD: 'openocd -f openocd.cfg &'..."
@echo off
"start cmd.exe /k C:\Program Files\OpenOCD\bin\openocd.exe" -f "openocd.cfg"
sleep 1

echo "Loading program"
@echo off
set TL_PATH="C:\Program Files (x86)\GNU Tools ARM Embedded\8 2019-q3-update\bin"
set PATH=%TL_PATH%;%PATH%

arm-none-eabi-gdb --batch --command=upload.gdb
echo.
echo Finished programming.
echo.

