@echo off
REM -------------------------------
REM Set path to JLink.exe
cd /d "C:\Program Files\SEGGER\JLink2"

REM Get current batch file's directory
set BATCH_DIR=%~dp0

REM Device info
set DEVICE=R7FA4E10D
set INTERFACE=SWD
set SPEED=1000
set SERIAL=53008565

REM Script and log file
set FLASH_SCRIPT=%BATCH_DIR%flash_script_53003523.jlink
set LOG_FILE=%BATCH_DIR%flash_log_DUT2.log

REM Run flashing
echo Starting flashing process...
JLink.exe -device %DEVICE% -if %INTERFACE% -speed %SPEED% -usb %SERIAL% -CommanderScript "%FLASH_SCRIPT%" > "%LOG_FILE%" 2>&1

REM Add confirmation to log
echo Serial Number: 53008565 >> "%LOG_FILE%"

echo Flashing complete. Log saved to %LOG_FILE%
exit