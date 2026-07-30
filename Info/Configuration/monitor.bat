@echo off
set "EXE_NAME=ResearchDownload.exe"
set "EXE_PATH=D:\River\DTU_EOL_24\EXEs\RESEARCHDOWNLOAD_R22.19.1701\RESEARCHDOWNLOAD_R22.19.1701_DUT1\Bin\ResearchDownload.exe"

:: Start the exe
start "" "%EXE_PATH%"

echo Monitoring %EXE_NAME% ...

:LOOP
timeout /t 2 >nul

:: Check if process is running
tasklist /FI "IMAGENAME eq %EXE_NAME%" | find /I "%EXE_NAME%" >nul

if errorlevel 1 (
    echo %EXE_NAME% has exited.
    goto END
) else (
    goto LOOP
)

:END
echo Closing CMD...
exit
