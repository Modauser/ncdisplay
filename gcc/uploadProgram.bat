@echo off
set TL_PATH="C:\Program Files (x86)\GNU Tools ARM Embedded\8 2019-q3-update\bin"
set PATH=%TL_PATH%;%PATH%

arm-none-eabi-gdb --batch --command=upload.gdb
echo.
echo Finished programming.
echo.
pause
