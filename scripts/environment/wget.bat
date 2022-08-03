REM 
REM Helper script to download an img file.
REM 

rmdir /S /Q img 2>nul
mkdir img

echo Downloading %1 ...
Powershell.exe -command "(New-Object System.Net.WebClient).DownloadFile('%1','img\%2')"

move img\%2 .