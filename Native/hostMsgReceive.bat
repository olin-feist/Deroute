@echo off

set LOG=%~dp0controller.log

time /t >> %LOG%

"node.exe" "%~dp0hostMsgReceive.js" %* 2>> %LOG%

echo %errorlevel% >> %LOG%
