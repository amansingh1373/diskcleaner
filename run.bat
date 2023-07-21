IF NOT EXIST "disk_cleaner.exe" (
	@ECHO OFF
	g++ main.cpp -o disk_cleaner.exe
)
@ECHO OFF
REM Run diskcleaner.exe with the specified arguments
"%~dp0disk_cleaner.exe" help

REM Optional: Pause the script to see the output before the Command Prompt window closes
PAUSE
