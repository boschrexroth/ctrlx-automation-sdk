@echo off
setlocal
SET WORKING_PATH=%CD%
SET "SDK_PUBLIC_PATH=%WORKING_PATH%\..\.."
SET "FBS2PLC_PATH=%SDK_PUBLIC_PATH%\bin\fbs2plc\win-x64\"
SET "SAMPLE_CPP_PATH=%SDK_PUBLIC_PATH%\samples-cpp"
SET "CPP_PROVIDER_ALL_DATA_PATH=%SAMPLE_CPP_PATH%\datalayer.provider.alldata"
SET "PLC_PATH=%SDK_PUBLIC_PATH%\plc"

@echo Arbeitsverzeichnis         : %WORKING_PATH%
@echo SDK_PATH                   : %SDK_PUBLIC_PATH%
@echo FBS2PLC_PATH               : %FBS2PLC_PATH%
@echo SAMPLE_CPP_PATH            : %SAMPLE_CPP_PATH%
@echo CPP_PROVIDER_ALL_DATA_PATH : %CPP_PROVIDER_ALL_DATA_PATH%
@echo PLC_PATH                   : %PLC_PATH%

SET PATH=%FBS2PLC_PATH%;%PATH%

fbs2plc.exe ^
-verbose ^
-plcKeep ^
-src %CPP_PROVIDER_ALL_DATA_PATH%\sampleSchema.fbs ^
-libTemplate %PLC_PATH%\CXA_fbs_Template.library ^
-libTitle fbs-sampleSchema ^
-libCompany MyCompany ^
-libVersion 1.0.0.0 ^
-libAuthor MyName ^
-libPlaceHolder SampleSchema ^
-libDescription "Function block to read/write flatbuffers described in sampleSchema.fbs." ^
-libDefaultNamespace sampleSchema

pause