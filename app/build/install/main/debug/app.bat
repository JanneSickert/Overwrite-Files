
@echo off
SETLOCAL

CALL "%~dp0lib\app.exe" %*
EXIT /B %ERRORLEVEL%
ENDLOCAL
