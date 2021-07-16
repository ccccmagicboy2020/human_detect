@echo off
for /f "delims=" %%a in ('JLink.exe burn.txt') do set a=%%a&&call :func1

:func1
if "%a%" == "Failed to open file."  (
    echo fail
	pause
)
