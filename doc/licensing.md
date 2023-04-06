This document is part of the Bosch Rexroth ctrlX CORE App Development Guideline, and describes how to adapt to the the license management of the device. 

Please refer to the App Development Guide for further information.

### **Table of Contents**

[1 Introduction](#introduction)

[2 License Enforcement](#enforcement)
> [2.1 Add Content Plug to snapcraft.yaml](#plug)

> [2.2 Adapt package-manifest.json](#manifest)

> [2.3 Use REST API to Enforce Licenses](#rest)

[3 Licensing Modes ](#modes)

> [3.1 Overview](#overview)

> [3.2 ctrlX CORE Main License](#main)

> [3.3 ctrlX CORE Temporary Usage Rights (Emergency Mode)](#emergency)

> [3.4 ctrX CORE 10 Day Trial License](#trial)

> [3.5 Four Hour Engineering Demo License on  ctrlX CORE<sup>virtual</sup>](#virtual)

> [3.6 Additional License Mode: ctrX CORE 3 Month Test License](#test)




# 1 Introduction <a name="introduction"></a>

Licenses are issued by the Bosch Rexroth Licensing Center [https://licensing.boschrexroth.com](https://licensing.boschrexroth.com). 

There a user can assign licenses to devices and can download a capability response, which contains all assigned licenses for a specific device. The downloaded capability response can than be deployed on the corresponding device.

The ctrlX CORE User Interfaces comes with a screen to upload the provided capability response and the possibility to check the currently available licenses on the device. The license manager feature on the ctrlX CORE offers a restful API via an internal unix domain socket, that can be used by Apps to request and release a specific license.

# 2 License Enforcement <a name="enforcement"></a>

In order to integrate license enforcement into your App, please follow the instructions below.

## 2.1 Add Content Plug to snapcraft.yaml (<a name="plug"></a>)
To get access to the unix domain socket that provides the restful API, add the following content plug to the snapcraft.yaml:

```yaml
plugs:
  licensing-service:
    interface: content
    content: licensing-service
    target: $SNAP_DATA/licensing-service
``` 

This will create a folder "license-service" during installation of the App on the ctrlX CORE and provide access to the unix domain socket "licensing-service.sock".

## 2.2 Adapt package-manifest.json (<a name="manifest"></a>)

The App shall provide a human readable text and a description with it´s package-manifest.json for each license that the App supports.
The "required" flag indicates, if the license is necessary for operation. Optional licenses (flag is false) extends the functionality of an application. 

``` json
"licenses": [
    {
      "name": "SWL-XCx-FRW-BASIC_FOOBAR-NNNN",
      "title": "ctrlX CORE - Foobar License",
      "description": "Unlock the App's functionality.",
      "required": true
    },
    {
      "name": "SWL-XCx-FRW-ADVANCED_FOOBAR",
      "title": "ctrlX CORE - Foobar License Extension",
      "description": "This license extends the functionality of the App.",
      "required": false
    }
  ],

  ```




## 2.3 Use REST API to Enforce Licenses <a name="rest"></a>


### 2.3.1 License Integration

A license model must be defined for each ctrlX CORE App, so that users have the same experience across all Apps when it comes to licensing. The App shall enforce licensing, according to the App's business model as part of the contract addendum.

Each software license (SWL) bought by a customer generates one or more capabilities for an App on a ctrlX CORE.
Every App has to aquire a license capability from the ctrlX CORE license pool when it starts, and while it is running to check if the license is activated or removed.
One or more licenses may be defined for a particular App.  In case of multiple licenses, each license has to be aquired separately (unique license ID for each license) from the ctrlX License Manager.

If the license is not needed anymore, it should be returned to the license pool. This is also necessary if an App is stopped or uninstalled. 
If an application crashes and the license was not returned properly, a reboot of the device is required. The reboot resets the license pool and all licenses can be aquired again. 

It is recommended to return the license back to the pool to re-aquire it again from time to time. So the App can detect if an license was removed or has expired. With this method, the App can detect license expiration very easily, and it does not need to handle expiration time by itself.

If a license is missing, an according warning or error must be shown, and the user should be informed, which licences are defined for the App.

 
!!! important
    On the ctrlX CORE, when enforcing licenses the use of the license management is mandatory. Individual implementations are not permitted.


### 2.3.2 License Manager API

The licensing API is available on GitHub: [https://boschrexroth.github.io/rest-api-description](https://boschrexroth.github.io/rest-api-description)

Implementing this API, an app is able to
- Acquire a license
- Release a license
- Get a list of all available license capabilities
- Update the license status in a ctrlX CORE


| Function | HTTP Method | URI | Payload | Response |
|---|---|---|---|---|
| Aquire license	| POST |/license| {<br>"name": "SWL-XCx-FRW-BASIC_FOOBAR-NNNN",<br>  "version": "1.0" <br>} | 200<br>{<br>  "isPermanent": true,<br>  "endDate": "2030-01-01T12:00:27.<br>7+00:20",<br>  "name": "SWL-XCx-FRW-BASIC_FOOBAR-NNNN",<br>  "id": <br>0083d875-30e8-4ff6-9f92-46096ef2d123",<br>  "version": "1.0",<br>  "startDate":"2030-01-01T12:00:27.87+00:20"<br>} |
|Release license|	DELETE|	/license/{id}	|none |	204	|
|Get list of all activated capabilities	|GET|	/capabilities	|none	|200,<br>[<br>{<br>"isPermanent": true,<br>    "finalExpirationDate": "2030-01-01T12:00:27.87+00:20",<br>    "name": "SWL-XCx-FRW-BASIC_FOOBAR-NNNN",<br>    "count": 1,<br>    "version": "1.0",<br>    "startDate": "2030-01-01T12:00:27.87+00:20"<br>  }<br>]<br>]
| Update the capability state in the device|	PUT|	/capabilities	|file<br>string($binary)



### 2.3.3 License ID

The API call to aquire a license generates a unique license ID for that particular license
name. This unique ID can be stored in temp file. 


> ![](images/licensing2.png)

Hint: Know more about mounting temporary disk in memory system type:tmpfs from snap documentation.


### 2.3.4 Capability Response

The capability response contains list of activated license capabilities for the particular ctrlX CORE device. The App shall verify / check the licenses, which are assigned to the App, with the licenses from capabilities from response.

Sample capability response:

> ![](images/licensing1.png)


# 3 Licensing Modes  <a name="modes"></a>

## 3.1 Overview <a name="overview"></a>

On a **ctrlX CORE** controller, an App specific license is provided in the ctrlX CORE License Manager, if an according license capability response file was uploaded to the ctrlX CORE.
The capability response file contains the App specific key, for example , and the License Manager returns information for this license key.

On a **ctrlX CORE<sup>virtual</sup>**, currently no licenses for productive use are available .
To allow a user to run and evaluate an App that enforces licensing on the ctrlX CORE<sup>virtual</sup>, the runtime on the ctrlX CORE<sup>virtual</sup> is limited to four hours

The following table lists the three different license mode on a ctrlX CORE and the ctrlX CORE<sup>virtual</sup>, together with additional information which is provided by the License Manager for the specific license type.



| License Mode | Target | Key | Usage | Additional information |
|---|---|---|---|---|
| Main License | ctrlX CORE |`SWL_XCR_{YOUR_APP_LICENSE_CODE}` | mandatory |(none specific)
| Temporary Usage Rights (Emergency Mode) | ctrlX CORE |`SWL_XCR_{YOUR_APP_LICENSE_CODE}` | mandatory |`tampered` flag is `true` |
| 10 Day Trial License |ctrlX CORE |`SWL_XCR_{YOUR_APP_LICENSE_CODE}` | optional |`isPermanent` flag is `false`
|Four Hour Engineering Demo License |ctrlX CORE<sup>virtual</sup>|`SWL_XCR_ENGINEERING_4H`|optional|(none specific)


These different license modes, and the expected behaviour of the App for these modes, are briefly described below.

## 3.2 ctrlX CORE Main License  <a name="main"></a>

**Purpose:** Standard License

**Description:**
- This is the standard license that a user obtains when buying an app. Therefore, the support of this license mode is **mandatory**.
- The license is obtained (bought) in the Bosch Rexroth Licensing Center and then assigned to a specific device by the user.
- The standard license can be either permanent (unlimited) or time based (expires)

**Expected App Behaviour**:
- The app shall start and run as intended, when the standard license is available.
- If acquiring a license is not possible (no license is available), the app shall inform the user that a license needs to be obtained.
- In case of time based licenses, the app should inform the user on time before the license expires. As a best practice, a user should be informed four weeks prior to expiration.
- When no standard app specific license is available on the device, an app shall only run if one of the other licenses is active.


## 3.3 ctrlX CORE Temporary Usage Rights (Emergency Mode)  <a name="emergency"></a>

**Purpose:** Avoid machine downtime

**Description:**
- On a ctrlX CORE, the user  has the possibility to activate a special mode that is called "Temporary usage rights". The mode can be activated using the License Manager UI.
- Enabling this mode allows users to operate the ctrlX CORE in a productive environment for ten consecutive days. The single purpose of this mode is to prevent a downtime when a defect device is replaced and no licenses have been assigned, yet. ==**This mode can only be activated once on a device**==
- Within these ten days, the user has to either add licenses to the new device or move the licenses from the broken device to the replacement device.

!!! warning
    After ten days, the mode will expire and cannot be reactivated.

**Expected App Behaviour:**

 - Support of this license is **mandatory**.
 - The App is informed with the `tampered` flag which is set to `true` in the response when acquiring such a license from the License Manager. 


## 3.4 ctrX CORE 10 Day Trial License  <a name="trial"></a>

**Purpose:** Test Mode on ctrlX CORE Hardware

**Description:**
- Users can aquire a 10 day test license for ctrlX CORE, which allows the usage of all ctrlX CORE Apps for this time.
- It is intended for test and evaluation only, and not for use in a production environment
- This license will automatically expire after 10 days.
- Differently from the Temporary Usage Rights (which is av ailable to avoid machine downtime in case of broken devices), several 10 Day Trial Licenses can be used one after another on a device, if an according contractual agreement is available for all involved parties (Bosch Rexroth, Customer, ctrlX World Partner).

**Expected App Behaviour:**
- Support of this mode is **optional**.
- The App is informed with the `isPermanent` flag set to `false` in the response when acquiring such a license from the License Manager. 


## 3.5 Four Hour Engineering Demo License on  ctrlX CORE<sup>virtual</sup> <a name="virtual"></a>

**Purpose**: Demonstration Mode on a virtual control (no ctrlX CORE hardware)

**Description**:
-  An according special license key (`SWL_XCR_ENGINEERING_4H`) indicates that currently the limited evaluation mode is active
- After 4 hours the virtual device shuts down itself to prevent a productive use.

**Expected App Behaviour:**
- Support of this mode is **optional**.
- In order to support this non-productive mode, Apps must check for the license `SWL_XCR_ENGINEERING_4H`, which guarantees that the runtime is limited to 4 hours.

##  3.6 Additional License Mode: ctrX CORE 3 Month Test License  <a name="test"></a>
**Purpose:** Time limited test and evaluation of Apps which are in beta/prototype phase (B-Sample phase)

**Description:**
- This mode is not intended / available for partner apps by default.
- Please get in touch with Bosch Rexroth directly, if this mode might be of interest for you.


**Copyright**
Copyright (c) 2022-2023 Bosch Rexroth AG