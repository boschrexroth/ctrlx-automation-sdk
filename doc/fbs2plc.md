# fbs2plc.exe - Use Flatbuffers in your PLC project

A lot of ctrlX Data Layer nodes contain structured data stored in the [Flatbuffers](https://google.github.io/flatbuffers/) format.

The data structure is defined in a schema file (.fbs file). The  FlatBuffers compiler [flatc](https://google.github.io/flatbuffers/flatbuffers_guide_tutorial.html) is able to compile such a .fbs file into code for several programming languages including IEC 61131-3 Structured Text (ST).

This guide describes how one or more fbs files can be compiled into IEC 61131-3 ST code and how this code can be imported and used in your IEC 61131-3 project to handle Flatbuffers.

The described workflow and the tools are currently running __only under Windows 10__.

## Prerequisites

### ctrlX WORKS

Install ctrlX WORKS >= 1.20 with the function 'ctrlX PLC Engineering' and 'App Build Environment'.

We recommend to use __"C:\Program Files\Rexroth\ctrlX WORKS"__ as installation path.
In this case the path to ctrlX PLC Engineering is __"C:\Program Files\Rexroth\ctrlX WORKS\Studio\Common\ctrlX-PLC-Engineering.exe"__ 

#### ctrlX CORE

* Create a ctrlX CORE<sup>virtual</sup> with __Port Forwarding__ and start it.
* Install the PLC snap.

### ctrlX AUTOMATION SDK

#### On your Windows host

Download the [ctrlX AUTOMATION SDK](https://github.com/boschrexroth/ctrlx-automation-sdk/releases) and extract it to __c:\ctrlx-automation-sdk__

Now two executables are available:

* c:\ctrlx-automation-sdk\bin\oss.flatbuffers\win-msvc-x64\release\flatc.exe
* c:\ctrlx-automation-sdk\bin\fbs2plc\win-x64\fbs2plc.exe

#### On your App Build Environment

* From ctrlX WORKS, create and start an App Build Environment.
* Login into your App Build Environment: ssh -p 10022 boschrexroth@127.0.0.1
* Password is boschrexroth
* Install the ctrlX AUTOMATION SDK

    $ ~/scripts/install-sdk.sh

* Build a required snap (here amd64) and install it on the ctrlX CORE<sup>virtual</sup>

    $ cd ~/ctrlx-automation-sdk/samples-cpp/datalayer.provider.all-data
    $ ../../scripts/build-upload-log-snap.sh -PF

* Open a web browser, login into your ctrlX CORE<sup>virtual</sup>. A Data Layer node __sdk-cpp-alldata/dynamic/fbs__ should exist.

This node later can be read, changed and written by our IEC 61131-3 PLC code.

## Reading and writing a flatbuffers ctrlX Data Layer node in your PLC project

The basic steps are:

1. Create a PLC library which contains the IEC61131 ST code to handle a Flatbuffers variable
2. Create a new PLC project
3. Import the created PLC library and provided sample code into your project
4. Compile the project and log into your ctrl CORE<sup>virtual</sup> 

### Create a PLC library

Start cmd.exe and enter these commands:

    c:
    cd \ctrlx-automation-sdk\samples-fbs2plc\sampleSchema
    start.bat

Result:
* The file c:\ctrlx-automation-sdk\samples-cpp\datalayer.provider.all-data\sampleSchema.fbs was compiled into IEC61131 code. 
* A new PLC library __fbs-sampleSchema__ was created, using c:\ctrlx-automation-sdk\plc\CXA_fbs_Template.library as template.
* All required standard libraries were imported.
* ctrlX PLC Engineering now has the new library opened.

Store the new library so that you can use it in your PLC project:

* Select File - Save project and install into library repository
* Close the libray
* Keep ctrlX PLC Engineering running

### Create a PLC project

* In ctrlX PLC Engineering create a new PLC project use 'ctrlX CORE x64 Project' as template.
* Select the node Application and delete it.
* Select the node PLC Logic
* Select menu item Project - PLCopenXML import..., select c:\ctrlx-automation-sdk\samples-fbs2plc\sampleSchema\fbs-read-write.xml 
* Double click the node library manager
* Add the library CXA_DATALAYER to your project.
* Connect the device of ypur PLC project with the ctrlX CORE<sup>virtual</sup> 
* Login into your ctrlX CORE<sup>virtual</sup> 

### Check the Flatbuffer Access

* From a web browser login into your ctrlX CORE<sup>virtual</sup> Web UI
* Select Settings - Data Layer
* Expand the Data Layer tree: sdk-cpp-alldata/dynamic/fbs

The values of the x, y, z variables should be changed by our PLC program.
