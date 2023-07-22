@ECHO OFF
IF NOT EXIST "diskcleaner.exe" (
    ECHO Loading...
    g++ main.cpp -o diskcleaner.exe
)
ECHO Loaded
@ECHO OFF
REM Run disk_cleaner.exe with the specified arguments
"%~dp0diskcleaner.exe" help

REM Optional: Pause the script to see the output before continuing
PAUSE

REM Allow the user to write commands in the same window
:UserInput
SET /P "userCommand=Enter a command (or type 'exit' to quit): "
IF /I "%userCommand%"=="exit" (
    EXIT /B
) ELSE (
    %userCommand%
)
GOTO UserInput