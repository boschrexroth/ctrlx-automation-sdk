REM 
REM Helper script to download an img file.
REM 

rmdir /S /Q img 2>nul
mkdir img

echo Downloading %1 ...
Powershell.exe -command "(New-Object System.Net.WebClient).DownloadFile('%1','img\%2')"

REM If using Powershell.exe is not possible:
REM - Download Wget for Windows e.g. from here: https://gnuwin32.sourceforge.net/packages/wget.htm 
REM - Install it - do not forget adding the storage location to the PATH environment variable.
REM - Disable the line calling Powershell.exe with am REM or delete it.
REM - Enable following line by removing 'REM '
REM wget.exe %1 -O img\%2

move img\%2 .