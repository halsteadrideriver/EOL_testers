@echo off
REM ====== User Configuration ======
set COM_PORT=COM35
set BAUD_RATE=921600
set PAC_FILE="D:\River\DTU_EOL_24\Info\Configuration\RX01_DTU_N58_Application_0_1_3-1.pac"
set CMDLOADER_PATH="D:\River\DTU_EOL_24\EXEs\RESEARCHDOWNLOAD_R22.19.1701\RESEARCHDOWNLOAD_R22.19.1701_DUT1\Bin\CmdDloader.exe"
set LOG_FILE="D:\River\DTU_EOL_24\EXEs\RESEARCHDOWNLOAD_R22.19.1701\RESEARCHDOWNLOAD_R22.19.1701_DUT2\Bin\result.txt"

REM ====== Start Flashing ======
echo.
echo =======================================
echo  Neoway Firmware Flashing Automation
echo =======================================
echo.
echo Flashing %PAC_FILE% on %COM_PORT%...
echo.

%CMDLOADER_PATH% -com %COM_PORT% -baud %BAUD_RATE% -pac %PAC_FILE% -start -log %LOG_FILE%

REM ====== Check Log for Success ======
findstr /C:"success" %LOG_FILE% >nul
IF %ERRORLEVEL% EQU 0 (
    echo.
    echo Flashing Successful!
) ELSE (
    echo.
    echo Flashing Failed. Check the log: %LOG_FILE%
)

exit