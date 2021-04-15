@echo off
for /f "delims=" %%a in ('JLink.exe burn.txt') do set a=%%a
if "%a%" == "Script processing completed."  (
	@echo on
    echo success
) else (
	@echo on
    echo fail
	pause
)
