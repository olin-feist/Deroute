@echo off

set LOG=%~dp0controller.log

time /t >> %LOG%

"node.exe" "%~dp0controller.js" %* 2>> %LOG%

echo %errorlevel% >> %LOG%
