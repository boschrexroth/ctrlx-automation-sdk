## Software Development Kit for ctrlX AUTOMATION

### Version 2.2.0 <small> August, 2023</small>

* Use base: core22 as basis for each sample project
* Bump sample projects to core22
* Use new ctrlX Data Layer with version 2.x

### Version 1.20.0 <small> March 10, 2023</small>

* Documentation and examples for the use of (external) media added
* Documentation how to use an extenable denylist

### Version 1.18.0 <small> November 30, 2022</small>

* Adapt bulk operation from ctrlX Data Layer
* Add new samples for bulk operations
* Update documentation

### Version 1.16.0 <small> July 15, 2022</small>

* Increase maximum numbers of opened files: Docu and sample project
* Convert Flatbuffers Schema Files to IEC61131 Code: Chapter in documentation Getting Started, PLC sample in samples-iec61131/fbs2plc
* Metadata database examples added to samples-cpp/datalayer.register.node and samples-python/datalayer.provider
* Revisions regarding Virtual Box and Native Ubuntu 20.04
* Deploy fbs only in namepaces

See also [Changes of the App Build Environment ](app_builder_env_changes.md)

### Version 1.14.0 <small> March 15, 2022</small>

* Use flatbuffers version 1.12 for c/c++ projects
* Use flatc 2.0
* Remove RT Bundles from SDK
* Restructured sample projects docu
* Quick Start Guide
* Using ctrlX WORKS function 'App Building Environments'
* Using Snapcraft Remote Build from Canonical

### Version 1.12.0 <small> November 22, 2021</small>

* Change samples to Ubuntu20
* Add QEMU as new recommended build environment
* Added node-package
* Added Node.js `Hello-Webserver` sample
* Added Node.js `ctrlX Data Layer Client`
* Added C++ `Automation Diagnosis`
* Added C++ `Automation Trace`
* Added C++ `ctrlX Data Layer Diagnosis`
* Added Samples for TPM2
* Added Developer Guidline
* Added description to adapt licensing mechanism
* Added `Bundle Description (doxygen)` for trace/diagnosis

### Version 1.10.0 <small> July 15, 2021</small>

* Added python `appdata` sample 
* Added python `ctrlX Data Layer Browse` sample 
* Added python `ctrlX Data Layer Client (complex)` sample 
* Added python `ctrlX Data Layer Client Subsrciption` sample 
* Added python `ctrlX Data Layer Provider all-data` sample 
* Added python `Webserver` sample 
* Added python `Calculation` sample 
* Refactored `hello.webserver` sample in go
* Added go `appdata` sample
* Added TPM2 sample in go and shell scripts 
* Adapt style
* Added Api Refernece c/c++ python and c#
* Improve Documentation
* Added Python Sample with different Loglevels
* Added C# Sample with different Loglevels
* Added C++ Sample with different Loglevels
* Remove REST API's from package (available online)

### Version 1.8.0 <small> March 25, 2021</small>

* Added .NET 5 (.NET Core) `hello.world` sample (self-contained)
* Added .NET 5 (.NET Core) `ctrlX Data Layer Browse` sample (self-contained)
* Added .NET 5 (.NET Core) `ctrlX Data Layer Client` sample (self-contained)
* Added .NET 5 (.NET Core) `ctrlX Data Layer Provider` sample (self-contained)
* Added .NET 5 (.NET Core) `ctrlX Data Layer Demo` sample (self-contained)
* Added python `ctrlX Data Layer Client` sample 
* Added python `ctrlX Data Layer Provider` sample 
* Added python API Documentation
* Added .NET 5 (.NET Core) API Documentation

### Version 1.6.0 <small> November 15, 2020</small>

* Changed license from BSD3 to MIT
* Refactored hello.webserver sample
* Added `datalayer.ecat.io` sample (exemplary work with fieldbus)
* Added API descriptions
* Extended `datalayer.register.node` and `datalayer.client` sample with flatbuffers
* Prepared experimental remote snap debugging
* Added `datalayer.realtime` sample (share real time data)

### Version 1.4.0 <small> July 15, 2020</small>

* Prepare first release
