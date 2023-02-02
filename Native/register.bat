@echo off

call :isAdmin

if %errorlevel% == 0 (
    goto :run
) else (
    echo Error: Run as administrator.
)

exit /b

:isAdmin
fsutil dirty query %systemdrive% >nul
exit /b

:run

reg add HKLM\SOFTWARE\Google\Chrome\NativeMessagingHosts\deroute_controller /f /ve /t REG_SZ /d %~dp0manifest.json

pause
