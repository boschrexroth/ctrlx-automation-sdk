@echo off
set scriptdir=%~dp0
set flatc=%scriptdir%..\bin\oss.flatbuffers\win-msvc-x64\release\flatc.exe
set lang=%1
set fbs=%~f2
set out=%~dp2

echo flatc: %flatc%
echo fbs:   %fbs%
echo out:   %out%
echo lang:  %lang%


%flatc% -b --schema --%lang% -o %out% %fbs% 