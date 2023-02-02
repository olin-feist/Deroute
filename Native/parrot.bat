@echo off

set LOG=%~dp0log.txt

time /t >> %LOG%

"node.exe" "%~dp0parrot.js" %* 2>> %LOG%

echo %errorlevel% >> %LOG%
