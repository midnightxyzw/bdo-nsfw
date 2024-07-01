@echo off
%~dp0.midnight_xyzw\elevate.exe powershell -Command (Set-ExecutionPolicy RemoteSigned)
powershell.exe %~dp0.midnight_xyzw\midnight_xyzw.ps1 %*
pause