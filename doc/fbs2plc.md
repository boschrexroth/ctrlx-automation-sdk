The ctrlX Data Layer provides nodes containing structured data in the [Flatbuffers](https://google.github.io/flatbuffers/) format.

Hereby in a Flatbuffer schema file (fbs file) the data structure is defined. The  FlatBuffers compiler [flatc](https://google.github.io/flatbuffers/flatbuffers_guide_tutorial.html) is able to compile such a fbs file into code for several programming languages including IEC 61131-3 Structured Text (ST).

This guide describes how a fbs file can be compiled into IEC 61131-3 ST code and how this code can be imported and used in your IEC 61131-3 project to handle Flatbuffers.

The described workflow and the tools are currently running __only under Windows 10__.

## Prerequisites

### 1. ctrlX WORKS

Install ctrlX WORKS >= 1.14 with the functions 'ctrlX PLC Engineering' and 'App Build Environment'. 

As installation path we recommend to use __"C:\Program Files\Rexroth\ctrlX WORKS"__

If you did so the path to ctrlX PLC Engineering is __"C:\Program Files\Rexroth\ctrlX WORKS\Studio\Common\ctrlX-PLC-Engineering.exe"__ 

### 2. ctrlX AUTOMATION SDK

Download the [ctrlX AUTOMATION SDK](https://github.com/boschrexroth/ctrlx-automation-sdk/releases) and extract it to __c:\ctrlx-automation-sdk__

The extracted archive contains two executables. Add the storage locations of these tools to the environment variable PATH. 

For flatc.exe:

    setx "%PATH%;c:\ctrlx-automation-sdk\bin\oss.flatbuffers\win-msvc-x64\release"


For fbs2plc.exe:

    setx "%PATH%;c:\ctrlx-automation-sdk\bin\fbs2plc\win-x64"

### 3. sdk-cpp-alldata as Provider

Reading and writing a Flatbuffer Data Layer node within our IEC 61131-3 code requires an instance which provides this node to the ctrlX Data Layer. Therefor we use the snap sdk-cpp-alldata deployed by the ctrlX AUTOMATION SDK in the folder __samples-cpp/datalayer.provider.all-data__. 

This snap has to be build and installed in the ctrlX CORE, see:

* [Quick Start Guide](setup_qemu_ctrlx_works.md)
* [Building Snaps](samples.md)

If the snap is installed and the ctrlX CORE is in Operation Mode it will provide a Flatbuffer variable with this Data Layer path:  __sdk-cpp-alldata/dynamic/fbs__

This node can be read, changed and written by our IEC 61131-3 sample code.

## From fbs File to a PLC library

### 1. Working Directory

Start a Windows the console app cmd.exe and create a working directory.

    c: & mkdir \fbs2plc & cd \fbs2plc

Copy the start.bat file from the ctrlX AUTOMATION SDK into this directory

    copy c:\ctrlx-automation-sdk\samples-iec61131\fbs-read-write\start.bat .

Copy the sampleSchema.fbs file from the ctrlX AUTOMATION SDK into this directory

    copy c:\ctrlx-automation-sdk\samples-cpp\datalayer.provider.all-data\sampleSchema.fbs .

### 2. ctrlX PLC Engineering

 Open ctrlX WORKS, select Engineering Tools and open ctrlX PLC Engineering.

 Within ctrlX PLC Engineering:

* Create a new empty library with the name sampleSchema
* Store it under c:\fbs2plc

### 3. Prepare start.bat

The batch file start.bat calls the executable fbs2plc.exe stored in c:\ctrlx-automation-sdk\bin\fbs2plc\win-x64
Open start.bat with a txt editor, check/change the parameter values below and save the file

    -title "Flatbuffer sampleSchema Handling" ^
    -company "Bosch Rexroth AG" ^
    -version 1.0.0.1 ^
    -released ^
    -author "Bosch Rexroth AG" ^
    -placeHolder "SampleSchema" ^
    -description "Makes sampleSchema.fbs available in PLC code" ^
    -defaultNamespace sampleSchema ^


*) If you do not want to release your library at once set this line under comment: 

`REM -released ^`

!!! important
    Do not delete the ^ character at the end of each line!

### 4. Run start.bat

Within the working directory launch the start.bat file: 

    start.bat

This file starts the fbs2plc executable. From here flatc is called to create ST code, which is then imported into the opened PLC library.

You can watch the progress in the trace output. If an error occurs the execution is stopped. Warnings can be ignored.

### 5. Check Imported Code

In ctrlX PLC Engineering check all imported content under the tab page __POUs__.

If everything is OK save the library: Main menu time __File: Store project and install in library system__

## Using the PLC library in a PLC Project

### 1. Create a new PLC Project

In ctrlX PLC Engineering

* Create a new Project,
* Use an according ctrlX CORE template.
* Name : sampleSchema 
* Save it under c:\fbs2plc

### 2. Add Library

* Double click Libray Manager
* Select Add Library
* Search the generated library: Flatbuffer sampleSchema Handling

### 3. Import PLC Code

* Select main menu item Project: Import
* Select the file __c:\ctrlx-automation-sdk\samples-iec61131\fbs-read-write\ctrlxVirtual.export __
* Click the Login icon in the tool bar (Alt + F8)
* The project will be complied and downloaded into the PLC runtime system of the ctrlX CORE - an existing PLC application in the ctrlX CORE will be overwritten.
* Start the PLC application with the start icon of the tool bar.

The PLC program should run without failure.

### 4. Check the Flatbuffer Access

* From a web browser login into your ctrlX CORE Web UI
* Select Settings - Data Layer
* Expand the Data Layer tree: sdk-cpp-alldata/dynamic/fbs

The values of the x, y, z variables should be changed by our PLC program.
