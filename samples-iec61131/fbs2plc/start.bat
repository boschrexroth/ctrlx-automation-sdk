@echo off
setlocal
SET PATH=c:\ctrlx-automation-sdk\bin\fbs2plc\win-x64\;%PATH%

fbs2plc.exe ^
-verbose ^
-plcKeep ^
-src c:\ctrlx-automation-sdk\samples-cpp\datalayer.provider.all-data\sampleSchema.fbs ^
-libTemplate c:\ctrlx-automation-sdk\plc\CXA_fbs_Template.library ^
-libTitle fbs-sampleSchema ^
-libCompany MyCompany ^
-libVersion 1.0.0.0 ^
-libAuthor MyName ^
-libPlaceHolder SampleSchema ^
-libDescription "Function block to read/write flatbuffers described in sampleSchema.fbs." ^
-libDefaultNamespace sampleSchema

pause