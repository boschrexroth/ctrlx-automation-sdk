
# How to protect content interface connections

## Introduction
 Each app can provide or consume multiple interface connections. The usage of some of these connections can destroy the integrity of the system.
 These connections are defined in the denylist which avoids an automatic connection by the Device Admin.

### Extend the denylist
 The app developer may want to protect own content interfaces and restrict them to specific apps. An example for such a connection is an unix socket to communicate
 between two apps (server and client). The functionality of the extended denylist was created for this purpose.

 The app should be able to indicate whether a particular content interface that it declares should be protected, and if so, what rules it can use.
 Therefore, the app developer can provide a denylist.json as a package asset.

 In order to make this possible, a few requirements must be met.

!!! attention
    - **The denylist.json must be part of the app and must not be changeable at runtime**
    - **The denylist.json must be part of the validation and checked by Rexroth**
    - **denylist.json is only accepted by Rexroth signed apps**
    - **Connectors must include partner/OEM prefix to avoid interferences**


## Requirements

 The extendable deny list is part of a customer app. Only signed customer apps can provide an own deny list, all deny lists in unsigned apps are ignored.

 The denylist.json file is located in the `package-assets` folder of the app, which is connected to the package-assets of the Device Admin.
 The *.package-manifest.json file is also located in this folder.


!!! example
    `$SNAP/package-assets/$SNAP_NAME/$SNAP_NAME.package-manifest.json`

 Only content interface (see: `https://snapcraft.io/docs/content-interface`) connections are supported! The content target has to contain the "parent" app name, e.g. the app "ctrlx-customer-app" can provide the protected content interface "ctrlx-customer-app-abc".

### Content interfaces
  Then there is a differentiation between content interfaces and all other interfaces.
  If the interface is a content interface the content property will be compared. If they where equal, the connection is allowed.

!!! info
    ```yaml title="plugs"

       ...
       plugs:
         ctrlx-customer-app-abc:
             interface: content
             content: ctrlx-customer-app-abc
             target: $SNAP_DATA/abc-folder
       ...
    ```
    ```yaml title="slots"
       ...
       slots:
         ctrlx-customer-app-abc:
           interface: content
           content: ctrlx-customer-app-abc
           source:
             write:
               - $SNAP_DATA/abc-folder-name/$SNAP_NAME
        ...
    ```

 Following this rule leads to a connection for the slot ctrlx-customer-app-abc with the plug ctrlx-customer-app-abc.

 The rules (location, deny list entries, ...) will be checked during the validation process.

## denylist.json

!!! example
    `$SNAP/package-assets/$SNAP_NAME/denylist.json`

 The denylist.json has the following format:

!!! info
    ```json title="denylist.json"

    [
        {
            "interface":  "content",
            "direction":  "slot",
            "parameters": "content=ctrlx-customer-app-data"
        },
        {
            "interface":  "content",
            "direction":  "plug",
            "parameters": "content=ctrlx-customer-app-additional-data"
        }
    ]
    ```
