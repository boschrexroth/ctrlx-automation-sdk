# ctrlX CORE App Development Guideline

**Liability** The information in this guideline is intended for product description purposes only and shall not be deemed to be a guaranteed characteristic, unless expressly stipulated by contract. All rights are reserved with respect to the content of this documentation and the availability of the product.

### **Table of content**
[1 Introduction](#introduction)

[2 App Validation Process](#validation)

[3 App Categories](#categories)

[4 Basic App Information (mandatory)](#basic)

[5 Further App Information (conditional)](#further)

[6 Working Set Overview (mandatory)](#workingset)

[7 ctrlX Basic Mechanisms (mandatory)](#basicmechansms)

[8 ctrlX Security System (optional)](#security)

[9 ctrlX User Interface and Project Handling (optional)](#userinterface)

[10 ctrlX AUTOMATION System Functions (optional)](#automation)

[11 Real Time Extension (optional)](#realtime)


## 1 Introduction<a name="introduction"></a>

This guideline give orientation to App developers and Partners who want to contribute with new Apps to the ctrlX World Ecosystem. 

This is a so-called "living document". New Apps might lead to new topics and may result in new guidelines at any time.

### 1.1 App development for ctrlX AUTOMATION
The open ctrlX AUTOMATION system architecture allows developers to implement further system functions with low effort at any time as Apps and as open source software. This document gives orientation on how to create apps to integrate best into ctrlX AUTOMATION.

With a dedicated app validation and signing process, Bosch Rexroth approves an App as **qualified for ctrlX AUTOMATION**. This process ensures that the app meets the quality standards of ctrlX AUTOMATION.

This document describes all aspects which have to be considered when an App is developed.
Some aspects are **mandatory**, both from legal and technical viewpoint. Others are **optional** and decribe how an App can be fully integrated in the ctrlX system architecture to provide the best user experience.

### 1.2 Obligation of technical aspects
This document describes technical aspects for ctrlX Apps. These aspects in each case have one of the following obligations:

 - <span style="color:red;">**MANDATORY**</span> - This aspect is stricly required and **must** be implemented
 - <span style="color:green;">**OPTIONAL**</span> This Aspect brings an added-value and **can** or **should** be implemented. 
 - <span style="color:blue;">**CONDITIONAL**</span>  - Aspect must be implemented in **special cases**. Bosch Rexroth and the Partner will identify the requirements during the App integration process.


### 1.3 Partner Contract and Legal Framework

This Development Guide describes the technical integration of an app into ctrlX. In addition, as non-technical prerequisites, the following documents need to be available and signed, before an app gets published:

- The **Software Distribution Framework** Agreement ("Partner Contract") describes fundamental constraints of the partnership between you as app developer and Bosch Rexroth and affiliates
- A **License Contract** describes all app related licenses as part of the product to be sold within Bosch Rexroth and affiliates sales channels
- Optionally a **Letter of Intent** (LOI)
- Optionally a **Non Disclosure Agreement** (NDA)

## 2 App Validation Process<a name="validation"></a>

As a mandatory step of an App integration into ctrlX, Bosch Rexroth will perform a standard App validation and signing process.

For handover, Bosch Rexroth provides an according ctrlX World Partner Portal SharePoint space, which will be prepared during the Partner contracting process.

### 2.1 Inital Meeting for the App Validation

Before the validation starts, the Partner / App Developer must provide the app architecture

**A picture of the main communication paths is needed, including**

* a building block overview of the App

* a short description of the communication paths to other Apps / Devices

* the App configuration and data storage concept

* a description of a typical standard scenario


The architecture overview should be stored as "**architecture-overview.x**" in the {app-name} folder and should be delivered with each new App version.

This document typically will be provided for an initial meeting with the Partner / App Developer, the according Bosch Rexroth Partner Manager and the Bosch Rexroth App validation Team.
At this point, the partner also should already have uploaded the necessary artifacts in order to be able to clarify any open points.

### 2.2 Artifacts Delivery ###
In an automated validation Framework, Bosch Rexroth checks the provided artefacts and the described behaviour of an App that is to be signed as official ctrlX App.

This validation Framework requires a standard format for artifacts and information.

To simplify the artifact's delivery, we provide a base folder structure with description and schema files. This will help App developers to deliver the required artefacts and us to simplify the validation work. Please use the given structure, folder names and file names like given below, without any renaming. 


The provided artifacts and information must be organized in the following way:

**General Folder structure**

- **{company-name}\\** - Partner Company Name. Folder name is created by Bosch Rexroth

    - **{app-name}\\**  - Technical App Name. This App name is unique. A folder will be created by the partner

      - **{version}\\** - The version folder separates the different versions. Format is three numbers separated by dots, e.g. "1.0.2". Each version has to be stored in a separate folder. A folder will be created by the partner

         - **{artifacts}\\** - The required documents as input for the validation process (see 4.1)


**Handover of artifacts**

After all required information is available in your local folder, zip the folder with Windows-zip and upload it as "**artifacts.zip**" to the ctrlX World Partner Portal space in the "**.../{company-name}/{app-name}/{version}/**" path.

!!! important 
    To avoid problems during upload of the artifacts.zip file, please use the Windows-zip function



### 2.4 Validation and Signing

The Validation typically will be done in several iterations, depending on the result of a particular validation activity.
If all required information is provided in the ctrlX World Partner Portal space, an according mail to `ctrlx.world@boschrexroth.de` will trigger a validation loop.
Basically, the workflow will look like the following:

1. Partner / App Developer: Upload required artefacts and inform Bosch Rexroth
2. Bosch Rexroth checks the artefacts for completeness. If rework is required, the partner will be notified via Email with an attached report in the "**.../{company-name}/{app-name}/{version}/results**" folder
3. When artefats are complete, the validation will start. Again, if rework is required, the partner will be notified with an attached report
4. When validation was sucessful, Bosch Rexroth will sign the App and inform the Partner via Email.



## 3 App Categories<a name="categories"></a>

Apps can be integrated into ctrlX on different levels. As an orientation for the prerequisites and possibilities on different levels, three categories are defined. A category, however, is no formal boundary, and an app can seamless support aspects of the different categories. Also, subsequent versions of an app might support more aspects than an earlier version.
In addition, there is no correlation between the category and the business value of an app.
However, at least all aspects that are mentioned in **Category 1** need to be met, since they are considered as **mandatory**.

In this chapter, the aspects are briefly described. The remainder of these documents provides detailed information about all aspects and also refers to additional sources of information, like how-to documents and code samples.

### 3.1 Category 1 (Basic): Apply to ctrlX Basic Mechanisms

To be approved as an official ctrlX CORE app, an app must support a minimum set of mandatory aspects.
#### **3.1.1 Overview**
The App itself and an according user documentation needs to be provided. Also legal aspects like FOSS are required.
Working set information is needed, to support the test and validation process.
During runtime, the App must use the onboard licensing mechanism of ctrlX CORE.
#### **3.1.2 Customer User Experience**
Customers find the App in the ctrlX App Store. They know how the App is licensed and can use the overall Bosch Rexroth licensing system for ctrlX.
They also can be sure that the App contributes to the basic ctrlX Security Mechanisms.

#### **3.1.3 Technical Prerequisites**

!!! note
    As mentioned, the aspects which are listed here are <span style="color:red;">**MANDATORY**</span>. Means, all of them are required for an official ctrlX app.

Basic App Information:

- App Artifacts (“Executables” for ctrlX CORE and ctrlX CORE virtual and basic technical information)

- App Documentation / User Manual and Release Notes

- Additional App Properties according to Linux Ubuntu conventions

- FOSS Information 


Working Set Overview:

- Information about Security behavior (Linux Slots & Plugs, Ports, Sockets)

- Information about Resource Consumption and Read/Write Operations

- Standard task scheduling overview

- Test Setup Description for typical usage scenario

Integration into ctrlX Basic Mechanisms:

- Use of ctrlX License Handling

- App signed by Bosch Rexroth



### 3.2 Category 2 (Advanced): Contribute to ctrlX Engineering Concepts
For a good integration into ctrlX from a user's point of view, an App should fulfil the aspects of Category 2 - even if these aspects are not necessary for App validation.

#### **3.2.1 Overview**
The App uses the according ctrlX system interfaces and supports ctrlX platform features like Identity Management, Data Management and Backup/Restore.

#### **3.2.2 Customer User Experience**
The App contributes to the ctrlX User Interface and System Behaviour. It integrates into basic user stories for configuration and maintenance of a ctrlX Application.

#### **3.2.3 Technical Recommendations**
!!! note
    As mentioned, the aspects which are listed here are <span style="color:green;">**OPTIONAL**</span>. However, they are highly recommended for a good user experience. Also, the aspects of category 2 and 3 are overlapping and not formally separated

Further App Information:

- FOSS Sources

- Semantic versioning scheme

Integration into ctrlX Security System:

- ctrlX Reverse Proxy

- ctrlX Key & Certificate Management

- ctrlX Identity Management

- Improved Network Security (no insecure protocols)

Integration into ctrlX User Interface and Project Handling

- ctrlX Navigation Pane and Landing Page

- ctrlX Configuration Storage


### 3.3 Category 3 (Extended): Extend ctrlX AUTOMATION features
Finally, an App can integrate in the Automation framework and extend the ctrlX real time system

!!! note
    Real time integration requires additional training and support from Bosch Rexroth to avoid unexpected system behavior and impacts on the ctrlX real time kernel.

#### **3.3.1 Description**
The App exposes information in the ctrlX Data Layer for all other Apps. It also can extend the real time functions of ctrlX if it connected to the real time task scheduler

#### **3.3.2 Customer User Experience**
The App extends the ctrlX AUTOMATION system functions, e.g. for Motion and/or PLC

#### **3.3.3 Technical Recommendations**

Integration into ctrlX AUTOMATION System Functions:

- ctrlX Data Layer

- ctrlX Logbook and Diagnostics System

Real Time Extension:

- ctrlX Scheduler


## 4 Basic App Information (<span style="color:red;">**MANDATORY**</span>) <a name="basic"></a>
The Basic App Information is checked as prerequisite by Bosch Rexroth, before the validation starts.

### 4.1 App Artifacts (<span style="color:red;">**MANDATORY**</span>)

#### 4.1.1 Artifacts folder template

The ctrlX World Partner Portal provides a file (artifacts.zip) which can be downloaded and extract locally.
This will create the required folder structure for the mandatory artifacts out-of-the-box, with default descriptions and schema files.

!!!! important 
     Please use the given structure, folder names and file names unchanged. This will support an efficient validation process.

![image](./images/artefacts.png)


The artifacts are organized in five sub folders:

#### 4.1.2 "Disclosure" folder (<span style="color:red;">**MANDATORY**</span>)

In the disclosure folder the FOSS source files and license text files are stored 

- **"fossinfo.json" (<span style="color:red;">**MANDATORY**</span>)** - license texts of all used open source software of an App. For more information about format and content, refer the example json and the according json-schema in the standard "artifacts.zip" file.

- **"foss-sources.zip" (<span style="color:blue;">**CONDITIONAL**</span>)** - In the foss-sources.zip file, the sources of all used open source libraries / packages are zipped without a password. 
- **"foss-offer.x" (<span style="color:blue;">**CONDITIONAL**</span>)** - If the foss-sources.zip file is not given, a human-readable file with the name "offer.x" is needed. It explains how the user can get the sources.

**Either FOSS sources or FOSS offer is required**.

#### 4.1.3 "Build Info" folder (<span style="color:red;">**MANDATORY**</span>)

The build info folder stores all build relevant information.

Note: For the `{xxx}-description` files, a default file (with explanation) and the according schema is provided in the standard `artifacts.zip` example.

- **"snapcraft.yaml" (<span style="color:red;">**MANDATORY**</span>)** - The snapcraft.yaml file is the main entry point to create a snap through Snapcraft. It contains all the details the snapcraft command needs to build a snap. See also https://snapcraft.io/docs/snapcraft-yaml-reference

- **"package-manifest.json" (<span style="color:red;">**MANDATORY**</span>)** - The package-manifest.json covers essential settings, like the proxy URL

- **"portlist-description.json" (<span style="color:red;">**MANDATORY**</span>)** - In the port list all used ports are described

- **"unixsocket-description.json" (<span style="color:red;">**MANDATORY**</span>)** - In the Unix socket description all used Unix sockets are  described. If your app don´t us a Unix socket, provide a empty description file

- **"slotplug-description.json" (<span style="color:red;">**MANDATORY**</span>)** - In the slot and plug description all used slot and plugs are  described

#### 4.1.4 "Documentation" folder (<span style="color:red;">**MANDATORY**</span>)
 
 Here, all App relevant documentation is provided 

 - **"manual.pdf" (<span style="color:red;">**MANDATORY**</span>)** - The App description (User Manual) documents the overall functionality of the App
 - **"test-setup-description.pdf" (<span style="color:red;">**MANDATORY**</span>)** - The App setup describes how to configure the App on a ctrlX device for a typical usage and test scenario
 - **"release-notes.pdf" (<span style="color:red;">**MANDATORY**</span>)** - In the release notes the newest changes, workarounds and defects are mentioned

#### 4.1.5 "App States" folder (<span style="color:red;">**MANDATORY**</span>)

The app validation framework tries to establish relevant states which have to be tested. Therefore, a sequence of API calls has to be provided to bring the App to the test state

- **"standard-scenario1.json" (<span style="color:red;">**MANDATORY**</span>)** - Each file contains a collection (sequence of Rest API calls) to generate the standard usage scenario. In each of these scenarios, the validation process monitors the memory and storage usage, the CPU load and the Ethernet communication load. The collections/files are to be created with Postman.

  - **"standard-scenario{2 to n}.json" (optional)** - one or more scenarios, if appropriate

#### 4.1.6 "Snaps" folder (<span style="color:red;">**MANDATORY**</span>)

ctrlX currently support the amd64 and the arm64 processor architecture. The according target snaps for the App are provided here

- **"{TechnicalAppName}_{version}_arm64.snap" (<span style="color:red;">**MANDATORY**</span>)** - Snap which runs in armd64 environments. Currently, the arm64 architecture is used in ctrlX CORE hardware.

- **"{TechnicalAppName}_{version}_amd64.snap" (<span style="color:green;">**OPTIONAL**</span>)** - Snap which runs in amd64 environments. The amd64 is used in ctrlX CORE virtual. However, future ctrlX CORE hardware will also use amd64 architecture. So, it is recommended to provide also a snap for this platform to avoid future inconvenience.



### 4.2 App Documentation (<span style="color:red;">**MANDATORY**</span>)

#### 4.2.1 User Manual (<span style="color:red;">**MANDATORY**</span>)
A user manual must be delivered together with the App.
The user manual must describe typical user actions for commissioning and operation of the App from a user / customer perspective.

#### 4.2.2 Test Setup Description (<span style="color:red;">**MANDATORY**</span>)

A test setup description must be delivered together with the App.
The test setup description must contain instructions to realize a test scenario.
It must include a description of sequences and dependencies, e.g. additional hardware. If you use plugs and slots, please provide an explanation here., 


#### 4.2.3 Release Notes (<span style="color:red;">**MANDATORY**</span>)

A release note documentation must be delivered together with the App.

#### 4.2.4 General App Description for the ctrlX App Zone (<span style="color:blue;">**CONDITIONAL**</span>)
Each app needs product information as part of the sales package. The description shall be generated based on the App description template (from the ctrlX World Partner Portal)

The App description package is necessary in case the App will be shown also in the App Zone of the ctrlX AUTOMATION Community or other marketing channels.

### 4.3 Additional  App Information according to Linux Ubuntu conventions (<span style="color:red;">**MANDATORY**</span>)

The following properties must be defined within the **"snapcraft.yaml"** file (see https://snapcraft.io/docs/snapcraft-yaml-reference), which also need to be exclusively part of the App and have to be collision free with other Apps.

- **Title** - The general name of the App, that will be shown in all sales channels and customer touch points, e.g. App overview or ctrlX App Store. Definition will be done together with the partner manager, as part of the business model definitions. Example: "My App"

- **Name** - The technical name of the snap. The name has to unique in the snap-universe and across all snap developer and device vendors. The snap name has to **start with "ctrlx-"** and must be **lowercase** and a **maximum length of 32 characters**. ctrlX World Partners add their company name to the snap name. Example: "ctrlx-partnername-myapp"

- **Confinement** - Must be set to **"strict"** for releases. See also https://snapcraft.io/docs/snap-confinement

- **Grade** - Defines the quality grade of the App. During development, you may choose to use **"devel"**. When releasing the application, the grade must be set to **"stable"**.


### 4.4 FOSS Info Provisioning (<span style="color:red;">**MANDATORY**</span>)
If the App uses Free and Open Source Software (FOSS), it is necessary to ship certain license information with the App.  

With **"fossinfo.xml"** Open source license text must be obliged to disclose for copyright reasons. Bosch Rexroth offers the possibility to display the license texts of the used open source software in the web interface of ctrlX, For more information about "fossinfo.xml" please refer to the guideline in the SDK. If the display of FOSS license texts is implemented within the App, at least a reference to the license display in the App must be provided in the "fossinfo.xml". 

Since users must be able to view the license texts before the open source software is installed, the "fossinfo.xml" must also be stored outside the App in the "disclosure" directory (see chapter 3.1.2). In addition, the license texts must be listed within the user documentation or at least a reference must be inserted where the FOSS license texts are located.


## 5 Further App Information (<span style="color:blue;">**CONDITIONAL**</span>)<a name="further"></a>
### 5.1 FOSS Sources (<span style="color:blue;">**CONDITIONAL**</span>)
Bosch Rexroth recommends putting all FOSS sources in a zip file with the file name **"foss-sources.zip"** and store it in the **"disclosure"** directory, where the **"fossinfo.xml"** is provided. 

In case that the FOSS sources are not directly provided, a written offer is mandatory for open source software with copyleft clause (e.g. GPLv2 or GPLv3) . This means, a human-readable file with the name "foss-offer.x" needs to be provided, which explains how the user can get the FOSS sources of the App.

### 5.2 Semantic versioning scheme (<span style="color:green;">**OPTIONAL**</span>)

It is recommended to use a versioning scheme which is based on the https://semver.org/ for versioning of the App. This setting is also relevant in the **snapcraft.yaml** file during creation of the snap.

!!! note
    Increasing version numbers are mandatory, independent of the used version scheme

### 5.3 Restart Delay (<span style="color:green;">**OPTIONAL**</span>)
The restart-delay of the App daemon should be set "5s" or similar in the **snapcraft.yaml** to prevent the 10s lock-out

## 6 Working Set Overview (<span style="color:red;">**MANDATORY**</span>)<a name="workingset"></a>

### 6.1 Security Information & Considerations

ctrlX CORE is designed with strong security requirements. Also, the overall ctrlX CORE architecture is built to be compliant to be certified as defined by IEC62433. This also implies some requirements to the Apps and software running in the ctrlX Ecosystem. For ctrlX App, this means

- Only encrypted and secured communication protocols (e.g. https) must be used. Unsecure protocols (e.g. http) are not to be used, unless explicitly defined in the validation process.

- The attack surface of the device should be kept as minimal as possible. This means for example that the number of open ports of an App should be reduced to a minimum.

#### 6.1.1 Interfaces / Slots & Plugs (<span style="color:red;">**MANDATORY**</span>)
Used interfaces (Slots & Plugs) must be documented in the **snapcraft.yaml** file. The following considerations must be taken into account:

- **Use as fewer interfaces (slots/plugs) as possible**. The amount of interfaces (slots & plugs) shall be **limited to a minimum**. The App should declare only the interfaces (slots and/or plugs) which are absolutely required to minimize the attack surface. When reviewing the App, Bosch Rexroth needs to know for which purpose a specific slot and/or plug is required by an App. The according justification must be provided together with the App. 

- **No global slots & plugs**. App developers must avoid assigning global slots & plugs, which are valid for all applications. Global slots & plugs should be used only if absolutely necessary.

- **Debug interfaces only on demand**. By default, the App should not provide any open network debug interfaces. If debugging is required, the user shall be able to enable the debug interface on demand and only after successful authentication (and authorization). In general, debug interfaces shall not be accessible without authentication and/or insufficient or even hard-coded credentials.


The following Operating System interfaces are denied or restricted for usage. See also https://snapcraft.io/docs/supported-interfaces.

| Interface   | Direction | Parameters | Exceptions | Reason |
| ----------- | ----------- | ---------| --------| ------|
| udisk2      | Plug & Slot |          |         |        |
| system-files      | Plug  |    *      |    "read=^(\\/dev\\/shm\| \\/dev\\/shm\\/.+)$", "write=^(\\/dev\\/shm\| \\/dev\\/shm\\/.+)$"     |        |
| snapd-control      | Plug | *          |         | Prevent uncontrolled access to system configuration.       |
|raw-volumes     |	Plug	| | | Prevent mounting of partitions.|
|process-control |	Plug	| | | Prevent tampering with running processes.|
|content	|Plug|	content=package-certificates	| |Prevent apps to act as device admin.|
|content |	Plug |	content=package-run	| | Prevent apps to act as device admin.|
|content|	Plug|	content=package-assets	| |Prevent apps to act as device admin.|
|content|	Slot|	content=auth-service	| |Prevent unauthorized access, service to service authentication.|
|content|	Slot|	content=active-solution	|  |Prevent apps to act as solution snap.|
|block-devices	| Plug	| | | Prevent mounting of partitions.|
account-control	| Plug	|* | | Prevent uncontrolled access to user and group databases.|
|removable-media|	Plug | | 	|Prevent uncontrolled access to removal media (micro sd, USB).|

 This list is subject to change and might be extended. If unsure, please provide the slot/plug you want to use inside your application together with a justification to check or not this specific slot/plug is allowed and find potential alternatives.

#### 6.1.2 Network Security and Ports (<span style="color:blue;">**CONDITIONAL**</span>)
The **package-manifest.json** must provide information about the used network interfaces.
The app must keep it's network footprint as low as possible:

- No open debug ports by default

- Binding of the webserver to unix socket (preferred) or at least localhost

Security protocols are to be be used by default. This means:

- No exposure of insecure protocols like http or websocket. ctrlX reverse proxy integration should be used for those specific protocols - or. in cases where that is not possible, https/websocket secure should be used in the App.

- For non-web Apps, secure protocols shall be used. When the App supports secure & insecure protocols, a secure configuration preset must be used for the App, so that the user must choose the override this setting if he wants to choose the insecure variant

- The App must provide a list of ports to be used in order to avoid conflicts. Thus, the App must be robust to already open / used ports. At least a warning shall be given to the user.

The following ports are blocked and cannot be used by an App:

- 22

- 80

- 81

- 443

- 1338 (UDP)

- 1880

- 1900 (UDP)

- 2069

- 2070

- 4840

- 5355 (UDP)

- 5353

- 6000

- 7878

- 8069

- 11740

- 11741

This list is subject to change and might be extended.


#### 6.1.3 File permissions (<span style="color:blue;">**CONDITIONAL**</span>)

It is very likely that an App stores settings & configuration data in the application's folder (e.g. $SNAP_DATA or $SNAP_COMMON). All file permissions have to be proper set so that only the owner of the files is allowed to read or alter the content.

### 6.2 Resource Consumption and Read/Write Operations (<span style="color:red;">**MANDATORY**</span>)

Typically, more than one App is running on a ctrlX device. Thus, it is very important than an App does not consume too much system resources (e.g. RAM or disk space). In addition, the available virtual memory on the device is limited to the amount of physical available memory, because the possibility of swapping unused RAM to disk is disabled on ctrlX devices. Reasons for this is the otherwise negative impact on realtime capability and flash disk lifetime.

#### 6.2.1 Resource Consumption

Recommended amount of resources per App:

- RAM: <75 MB

- Snap-Size: ideally <100 MB, as small as possible

If an App exceed these values by far, then please get in contact with your App partner support for further clarifications.

#### 6.2.2 Integrated storage / Flash lifetime
The App must not write diagnostics or similar data cyclically to the internal solid-state memory, as this will damage the flash cells. Instead, cyclical writing can be only be done to a network storage or any other external storage, as these allow easy and regular replacement for this use-case.

The integrated storage medium and file system in the ctrlX CORE hardware is based on a solid state flash memory, which has by its nature a limited lifetime based on the number of erase cycles of its memory cells. In order to increase the overall lifetime of the device it is necessary to reduce the number of write/erase cycles on the flash cells.

### 6.3 Standard task scheduling (<span style="color:red;">**MANDATORY**</span>)

The App must not have a negative impact for the realtime behavior of other apps. Scheduling / task scheme must stay flexible to allow to operate other apps, e.g. the ctrlX Motion App.

### 6.4 Test Setup for typical usage scenario (<span style="color:red;">**MANDATORY**</span>)

For a fast and efficient start of the validation process, a usage scenario should be provided which does not require any peripheral components. This will help to identify first findings quickly and with rather low effort. If the App requires additional periphery for proper operation, a separate usage scenario should be provided which describes the interaction with the external component.

Videos and further media can be attached.

## 7 ctrlX Basic Mechanisms (<span style="color:red;">**MANDATORY**</span>)<a name="basicmechanisms"></a>

### 7.1 ctrlX License Handling (<span style="color:red;">**MANDATORY**</span>)

A license model must be defined for each App which is part of ctrlX World. To ensure the maximum usability for a ctrlX CORE user, the App must call the ctrlX license manager API on start or in operation to check if a license is activated.  If a license is missing, the missing license will be shown to the user via a user interface (Note: This will be available after July 2021). A warning or error must be shown in case the license is missing.

Each software license (SWL) bought by a customer generates one or more capabilities:

- Each App checks the existence of these capabilities, using the license manager interface

- The enforcement of an App license shall be realized according to the App business model as part of the contract addendum

- The usage of other licensing mechanisms is not allowed

- Licensing Interface will be available by March 2021


!!! note
    Currently, the License Manager returns no licenses on a ctrlX COREvirtual.

### 7.2 App signing by Bosch Rexroth (<span style="color:red;">**MANDATORY**</span>)

To make sure that only Apps, which have successfully passed the validation process, are available as ctrlX App, these Apps need to be signed by Bosch Rexroth.
During the signing process, the App binaries are checked and combined with a signature. This signature guarantees that the App cannot be modified after the validation and signing process.

!!! note
    Currently, only Bosch Rexroth can sign ctrlX Apps

## 8 ctrlX Security System (<span style="color:green;">**OPTIONAL**</span>)<a name="security"></a>

### 8.1 ctrlX Reverse Proxy (<span style="color:green;">**OPTIONAL**</span>)

The proxy URL is the most important setting in the **package-manifest.json** file, so that the App is integrated seamlessly into the ctrlX CORE. The reverse proxy will forward requests to the URL that are defined for the App web service.

An App shall be bound to a unix socket instead of a local port. This will also prevent potential collisions with other services.

!!! warning 
    We strongly recommend binding the App web service to a unix socket, e.g. `/var/snap/my-app/current/package-run/my-app/my-app.web.sock`, instead of a local port. This will also prevent potential collisions with other services. The path length of a unix socket is limited to 108 characters. The complete path must respect that limit and the highlighted part has a maximal length of 50 characters. The path can be shortened if necessary by replacing "package-run" with "run" and shorting the file name from "my-app.web.sock" to "web.sock". However the folder name within the package-run (or run) folder must always be the name of the snap.

!!! note
    Example code can be found here:
    https://developer.community.boschrexroth.com/t5/ctrlX-WORKS/Example-IntegrationWebServer-to-Reverse-Proxy/m-p/20193#M216


### 8.2 ctrlX Key & Certificate Management (<span style="color:green;">**OPTIONAL**</span>)

The ctrlX CORE certificate management shall be used in order to manage the certificates of your application via the web interface.
(package-manifest.json, snapcraft.yaml)

Note: When the application acts as a ctrlX client, key & certificate based authentication has to be used whenever possible. In case if user & password based authentication is used, the configuration file permissions have to be correct so that no other user(s) on the system can read those data.

### 8.3 ctrlX Identity Management / Authentication & Authorization (<span style="color:green;">**OPTIONAL**</span>)

When running a web service, the App should use the authentication & authorization mechanisms that the ctrlX CORE provides to protect the App against unauthorized access.

If the mechanisms the ctrlX CORE provides are not used, hardcoded accounts / credentials have to be avoided.

The ctrlX CORE's so called "scopes" shall be used to model App permissions. Scopes allow the assignment of permissions to users & groups via the web interface. In case permissions are used, the enforcement of the permissions must be implemented within the App
(package-manifest.json)


## 9 ctrlX User Interface and Project Handling (<span style="color:green;">**OPTIONAL**</span>)<a name="userinterface"></a>

### 9.1 ctrlX Configuration Storage (<span style="color:blue;">**CONDITIONAL**</span>)

The ctrlX CORE system provides multiple ways for an App to store its data. In order to provide a consistent backup and restore mechanism to users, the App data must be persisted within one of these locations.

#### 9.1.1 Solution Handling (<span style="color:green;">**OPTIONAL**</span>)

The so-called "Solution" is an essential part of any ctrlX CORE device. It provides a central storage location for all ctrlX Apps, that need to persist their app data in configurations. Configuration files are saved to the solution storage or loaded from the storage on demand (by the user or by REST API calls).

- A Configuration file contain project-specific data, like machine-related, PLC, HMI, fieldbus configurations.

- Shall be used in case of one or more configurations that can be easily switched by the user. Configurations are used and shared by multiple apps.

- It does not contain data that is related to a specific device, e.g. certificates, network configurations, users and their permissions, or should be valid for multiple configurations (e.g. app settings).

- Should be non-binary (in future version control system / GIT will be used). However, If there is no alternative and the files are not changing too often, small binaries can be stored in a configuration.


#### 9.2.2 Environment Variables (<span style="color:blue;">**CONDITIONAL**</span>)

Environment variables are widely used across Linux to provide convenient access to system and application properties (see also https://snapcraft.io/docs/environment-variables)

In the specific context of ctrlX, the environment variables **$SNAP_COMMON** or **$SNAP_DATA**...

- ...Contain App-related data, that can be used across multiple configurations, e.g. solutions.

- ...Shall not contain data related to the user application, e.g. machine program, and should be applicable on multiple configurations.

- $SNAP_DATA shall be used for App data related to a specific app version (snap revision).

- $SNAP_COMMON shall be used App data across versions (snap revisions).

#### External storage / SD Card and USB storage device (not available yet)

!!! note
    These external storage devices are not supported at the moment.


In the future, they

- Can contain large data (>100MB) or data that is frequently changed, e.g. logs, analytics, database.

- Must contain only non-confidential (public) data or data must be protected on app side. Data can be accessed or manipulated from external.


## 10 ctrlX AUTOMATION System Functions (<span style="color:green;">**OPTIONAL**</span>)<a name="automation"></a>

### 10.1 ctrlX Logbook and Diagnostics System
For further information see https://docs.automation.boschrexroth.com


## 11 Real Time Extension (<span style="color:green;">**OPTIONAL**</span>)<a name="realtime"></a>

Please contact your Partner Manager if a real time extension might be required for your App.


**Copyright**
© Bosch Rexroth AG 2021
This guideline, as well as the data, specifications and other information set forth in it, are the exclusive property of Bosch Rexroth AG. It may not be reproduced or given to third parties without our consent.
