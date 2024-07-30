## Basics

Beside the metadata published using the [snapcraft.yaml](https://snapcraft.io/docs/snapcraft-yaml-reference) the package-assets interface provides a way to provide additional information of your app to the ctrlX CORE system. Currently the following information can be provided by the package-assets folder:

- **A package manifest** containing a declarative description of the app, including permissions, menus and reverse proxy configurations
- **Translation files** for different parts of the app
- **Open Source Software (OSS) information** of used Software in the app

Details for each topic can be found below.

## Providing the Package Assets

The third-party app has to provide the manifest file to the ctrlX CORE system. Thus, a data exchange has to be set up between the app and the Device Admin app. To exchange information, the content interface with the name "package-assets" is used. This content interface works opposite to the "normal" content interface. In this case, the plug provides data by mounting a directory into the space provided by the slot. Canonical calls this principle "spool mode".

The following example shows the procedure step-by-step.

1. Create the folder "configs/package-assets” in the root of your snap project.
2. Update your snapcraft.yaml
    * Add a new part which copies the content of the "configs" directory to your $SNAP folder using the dump plugin.
    * Add the slot "package-assets" to your slot configuration
3. Add the relevant files to "configs/package-assets" folder, for detailed information on each file see below.

!!! Example

    ```yaml title="snapcraft.yaml"
    name: rexroth-myapp
    [...]
    parts:
      configs:
          plugin: dump
          source: ./configs
          organize:
            'package-assets/*': package-assets/${SNAPCRAFT_PROJECT_NAME}/
    [...]
    slots: 
      package-assets:
          interface: content
          content: package-assets
          source:
            read:
            - $SNAP/package-assets/${SNAPCRAFT_PROJECT_NAME}
    [...]
    ```

## The package manifest

An app can provide own menu entries, use the reverse proxy infrastructure including permissions and the storage solution. This document describes the different integration points and how they work.

The package-manifest is essential for the integration points. Beside the snapcraft.yaml, it is strongly recommended that the app has to provide the package manifest to be integrated into the ctrlX CORE system. The package-manifest file should follow the naming structure `<app name>.<package-manifest>.json` where `<app name>` and the `id` property in the package-manifest are both set to the `name (technical app name)` as mentioned in the [snapcraft](appdevguide_basechecks.md) documentation.

The package-manifest file name should follow the pattern: 

    <snap name>.package-manifest.json

The _snap name_ is the _name_ of your app, configured by the _snapcraft.yaml_.

### Menus

An app can be integrated into the ctrlX CORE navigation by defining menu entries. There are three possible integration points:

* Sidebar: To allow the navigation to the main functionality of your apps
* Settings: To allow the navigation to your app settings
* Home Overview: To provide a customized tile for your application
  
![Integration](samples-go/webserver/docs/images/integration.png)

### Declare menu items in the package-manifest

Add a "menus" element to the package manifest of your snap. The "menus" element is defined as an object that comprises three elements:

* sidebar: An array of items to be integrated into the main menu
* settings: An array of items to be integrated into the settings menu
* overview: An array of items to be integrated into the app overview

An item is defined as an object with the following elements:

* id: A string used as unique item identifier to order items (required)
* title: A string used as display title of the item (required)
* description: A string providing an additional description (optional and only used by overview items)
* icon: A string representing the icon name of the item (optional)
* link: A string representing the target address of this item, including the application base href (required if no "items" are provided)
* target: A string that specifies where the link is shown. Use "_blank" or a specific name to open the link in a new tab (recommended for 3rd-party apps)
* items: An array of sub-items of this item (required if no link is provided. It is currently only supported for main menu items and restricted to one sub-level)
* permissions: An array of scope identifiers representing the permissions required to enable the item (optional)

??? example 

    ```json
    {
      "$schema": "https://json-schema.boschrexroth.com/ctrlx-automation/ctrlx-core/apps/package-manifest/package-manifest.v1.3.schema.json",
      [...]
      "menus": {
        "sidebar": [
          {
            "id": "solutions",
            "title": "Solutions",
            "icon": "bosch-ic-solutions",
            "permissions": [
              "solutions.r",
              "solutions.rw"
            ],
            "items": [
              {
                "id": "solutions-activeconfiguration",
                "title": "Active Configuration",
                "link": "/solutions/activeconfiguration",
                "permissions": [
                  "solutions.r",
                  "solutions.rw"
                ]
              },
              {
                "id": "solutions-overview",
                "title": "Solutions Overview",
                "link": "/solutions/solutions",
                "permissions": [
                  "solutions.r",
                  "solutions.rw"
                ]
              }
            ]
          }
        ],
        "settings": [
          {
            "id": "solutions-settings",
            "title": "Solution Settings",
            "icon": "bosch-ic-gear",
            "link": "/solutions/settings",
            "permissions": [
              "solutions.rw"
            ]
          }
        ],
        "overview": [
          {
            "id": "solutions-overview",
            "title": "Solutions",
            "description": "Manage solutions on the control",
            "icon": "bosch-ic-solutions",
            "link": "/solutions/activeconfiguration",
            "permissions": [
              "solutions.r",
              "solutions.rw"
            ]
          }
        ]
      },
      [...]
    }
    ```


If the third-party app uses a specified port and not our reverse proxy infrastructure, use the template variable "${hostname}" to allow the ctrlX CORE to replace it dynamically. E.g.:

!!! example

    ```json
    [...]
              {
                "id": "node-red",
                "title": "Node-RED",
                "link": "http://${hostname}:1880/",
              }
    [...]
    ```

If you want to open your application in your own tab, you can define the target tab. After this integration, a third-party app opens in a separate tab in the browser. If your application can integrate and validate the ctrlX CORE token. You can "transfer" the token using a query field. This is done with the template variable "${bearertoken}".

??? Example

    ```json
    {
      [...]
      "menus": {
        "sidebar": [
          {
            "id": "app-name",
            "title": "My App Name",
            "icon": "myApp-Icon",
            "permissions": [],
            "items": [
              {
                "id": "myApp-Id",
                "title": "MySideBarTitle",
                "link": "/app-name?access_token=${bearertoken}",
                "target":"myApp-Id",
                "permissions": []
              },
              [...]
            ]
          }
        ]
      },
      [...]
    }
    ```

### Dashboard Widgets

You can provide own widgets to be shown in the Home screen. These widgets need to be implemented as [Web Components](https://developer.mozilla.org/en-US/docs/Web/API/Web_components).

In short, widgets are provided in a JavaScript file and define a custom HTML tag. This JavaScript file can be loaded into any web page and the widget is instantiated by just inserting its tag into an HTML file (just like standard tags like \<div> or \<table>). One JavaScript file can contain several Web Components. Your app must serve this JavaScript file, just like other assets, so that it can be addressed via a URL.

For the Home screen to know about all the widgets, these need to be made publicly known in some way. This is done in the app's manifest in "uiExtensions" → "dashboard". Each element here defines one widget by providing the information needed to instantiate the widget (like the URL of the JavaScript file, where the widget is implemented, and the widget's tag name, size information) and metadata to display in the widget catalog. For a detailed description see the PackageManager-API, schema "DashboardWidget".

Example
  ```json
  "uiExtensions": {
    "dashboard": [
      {
        "order": 10,
        "src": "/motion/widgets.js",
        "tag": "rexroth-axes-widget",
        "title": "Axes",
        "description": "Shows the status of your axes",
        "categories": ["Motion"],
        "i18n": "dashboard.motion.axes",
        "image": "/motion/assets/img/DC_AE_ctrlX_DRIVE-axes_Symbol_bl_192x192_202102.svg",
        "initialSize": {
          "rows": 3,
          "cols": 3
        },
        "minSize": {
          "rows": 3,
          "cols": 3
        },
        "maxSize": {
          "rows": 3,
          "cols": 3
        }
      }
    ]
  }
  ```

### Reverse Proxy

The reverse proxy handles all **external** web-based (HTTP/HTTPS) requests to the device. The reverse proxy provides the following functionality:

* **Automatic HTTPS:** By default, all data exchanged between client and server is encrypted.
* **Token validation:** By default, only authenticated users may access resources on the device (except the login page). Requests with invalid tokens (lifetime expired, invalid signature) are rejected.

The proxy mapping provides the configuration required by the reverse proxy to redirect requests for a specific URL to a web server running on the device.

**Proxy Mapping**

!!! Example

    ```json
    {
      [...]
      "services": {
        "proxyMapping": [
          {
            "name": "rexroth-solutions.web",
            "url": "/rexroth-solutions",
            "binding": ":5000",
            "restricted": [
              "/rexroth-solutions/api/v1.0"
            ]
          }
        ]
      },
      [...]
    }
    ```

Providing a proxy configuration is optional in some cases (e.g. if your snap does not provide a web service). If a proxy configuration is provided, the following parameters are valid:

* **name (mandatory):** Name of the web service.  The format is: `<id>.<service>`The name has to be unique. Examples:
  * `rexroth-solutions.web`
  * `rexroth-hmi.web`
  * ...
* **url (mandatory):** URL provided by the reverse proxy. When a client tries to access this URL, the reverse proxy transparently redirects the request to the web server. Thus, your web server has to be configured to listen to e.g. "/cool-app" and not "/". Must not conflict with other web service URLs. Examples:
  * /solutions
  * /hmi
  * ...
* **binding (mandatory):** Resource identifier to which the reverse proxy redirects the requests. The resource can either be a port or a Unix socket. Using a Unix socket is highly recommended. Examples:
  * :5000
  * 192.168.1.17:5000
  * "unix://{$SNAP_DATA}/package-run/rexroth-solutions/rexroth-solutions.web.sock"
* **restricted:** Restricted URL. Only authenticated clients can access the URLs defined here. One important example is the API URL(s) - those URLs should be restricted in most use cases. Examples:
 "/rexroth-solutions/api/v1.0"

!!! info
    **Remark:** If a snap provides multiple web servers, multiple ProxyMapping entries are allowed.

#### Binding

**Unix sockets (recommended)**

To use Unix sockets, the web server of your application has to bind against a [Unix Domain Socket](https://en.wikipedia.org/wiki/Unix_domain_socket). Most web server frameworks already support this functionality. The benefit of Unix sockets is the additional security (file access can be restricted via file permissions) and the lower attack surface on network level (the service cannot be reached externally). To enable your snap and to provide the reverse proxy access to your Unix socket, use the content interface slot "package-run". It uses the same mechanism as the "package-assets" interface.

!!! example
    ```yaml
    [...]
      package-run:
        interface: content
        content: package-run
        source:
          write:
          - $SNAP_DATA/package-run/${SNAPCRAFT_PROJECT_NAME}
    ```

In the example mentioned above, your "unix socket file" should be created under `{$SNAP_DATA}/package-run/{$SNAP_NAME}/web.sock`. The file has to be managed completely by your application logic and - in contrast to a network socket, has to be deleted after unbinding. Also consider deleting the Unix domain socket before binding your web server before start-up.

Now, publish the information on the socket file using the package-manifest via the ProxyMapping described above. The relevant part is the "binding" section below. Replace

!!! warning
    The path length of a Unix socket is limited to 108 characters. Thus, ensure that the resolved path, e.g /var/snap/third-party-snap/current/==package-run/third-party-snap/third-party-snap.web.sock== does not exceed that limit and that the highlighted part does not exceed 50 characters. If required, shorten the path by replacing "package-run" with "run" and change the file name from "third-party-snap.web.sock" to "web.sock". Example:

!!! Example

    ```json
    [...]
    "services": {
        "proxyMapping": [
        {
            "name": "third-party-snap.web",
            "url": "/cool-app",
            "binding": "unix://{$SNAP_DATA}/package-run/third-party-snap/web.sock"
        }
        ]
    },
    [...]
    ```

**Local host (not recommended)**

Only bind to a local host if a Unix domain socket (see above) cannot be used. Your app has to bind to a specified port that is not used by other apps.

!!! Example

    ```json
    [...]
      "services": {
        "proxyMapping": [
          {
            "name": "third-party-snap.web",
            "url": "/cool-app",
            "binding": ":4711",
          }
        ]
      },
    [...]
    ```

### Scopes and Permissions

If access is restricted to specific resources, define scopes. When using the Identity Management, an administrator can configure the users and groups allowed to access specific resources. The following example shows two simple scopes (read/write, all settings, read-only settings). In every routine, the web server has to check whether the provided token includes the scope.

??? example

    ```json
    {
      "id": "rexroth-solutions",
      [...]
      "scopes-declaration": [
        {
          "identifier": "rexroth-solutions.web",
          "name": "Solutions Web Server permissions",
          "description": "Solution Management Permissions",
          "scopes": [
            {
              "identifier": "rexroth-solutions.web.solutions.rw",
              "name": "Solutions Management",
              "description": "Manage and modify solutions"
            },
            {
              "identifier": "rexroth-solutions.web.solutions.r",
              "name": "View solutions",
              "description": "View (but not modify) solutions"
            }
          ]
        }
      ],
    ```

**Remark: Restrictions apply on the scope namings, identifiers, etc. (see below)**

Use the following schema for the main identifier of the scope: 

`<id>.<service>`

* id: Package identifier. Has to correspond to the snap name
  * Example: rexroth-solutions, rexroth-vpnmanager, ...
* service: Service identifier
  * Example: web

Some examples:

* rexroth-solutions.web
* rexroth-vpnmanager.web

Use this schema for the identifier of the scope: 

`<id>.<service>.<scope_name>.<access>`

* id : Package identifier. Has to correspond to the snap name
  * Example: rexroth-solutions, rexroth-vpnmanager, ...
* service: Service identifier
  * Example: web
* scope_name: Name of the specific scope
  * Example: solutions, connections, configurations, users, ...
* access: Type of access to the resource represented by this scope. Supported are:
  * r (read-only): Allows read-only access to specific resources
  * w (write-only): Allows write-only access to specific resources
  * rw (read/write): Allows read-only and write access to specific resources
  * x (execute): Executes a specific action represented by a resource

Some examples:

* rexroth-vpnmanager.web.shortcuts.r: Read-only access to VPN shortcuts
* rexroth-vpnmanager.web.shortcuts.rw: Read and create VPN connection shortcuts
* rexroth-deviceadmin.web.own_password.w: Update (but not read) the password of the user currently logged in
* rexroth-vpnmanager.web.shortcuts.x: Start/stop VPN connections via existing starters (shortcuts)

#### Enforcing permissions in your service

The web server has to enforce the defined permissions. The following pseudo code snippet shows a simple example in which the function ListAddresses checks whether the client has the permission to execute the function. The scope "rexroth-device.all.rwx" is a special scope reflecting an administrative access permission (see below). 

!!! Example

    ```golang
    func ListAddresses(w http.ResponseWriter, r *http.Request) {
        
        // Define valid scopes for resource access
        scope := []string{"networkmanager.all.r", "networkmanager.all.rw", "rexroth-device.all.rwx"}
    
    
        // Extract scopes from http request and compare them to valid scopes
        // Return if not authorized
        if ok, _ := checkPermissions(r, scope); !ok {
            errors.ErrUnauthorized.Write(w)
            return
        }
    ```

#### Admin Scope

By default, the Identity Management provides a special admin scope ("rexroth-device.all.rwx”) that should only be assigned to admin users.  It is recommended to grant users full access to your web application if they are provided with this permission. This facilitates the handling of administrator accounts.

The following snippet shows the definition of the admin scope. You can see (and assign) the permission to users and/or groups.

!!! Example

  ```json
    {
        "identifier": "rexroth-device",
        "name": "Global device administration",
        "description": "Global device administration permissions allow unrestricted access to all system resources",
        "scopes":[
            {
                "identifier": "rexroth-device.all.rwx",
                "name": "Administration access",
                "Description": "Unrestricted, administrative access to system resources"
            }
        ]
    }
  ```

### Licenses

The app shall provide information in the package-manifest about each license that is supported. Description and title shall be human readable.
The "required" flag indicates whether a license is mandatory to use the application. Set the flag to true when the license is required and to false, when the license is optional. Multiple licenses can have the "required" flag set to true simultaneously. In this case, the app must ensure that it works if any of the 'required' licenses is available.

!!! Example
  ```json
  "licenses": [
      {
        "name": "SWL-XC*-RUN-DLACCESSNRT**-NNNN",
        "title": "ctrlX OS License - Customer App",
        "description": "Integration of customer apps into ctrlX OS with access to the ctrlX Data Layer",
        "required": true
      }     
    ],
  ```

### Certificate Management (optional)

The certificate manager handles cryptographic material for all applications on the device. The certificate manager can add/delete/list certificates or keys to/from a specific application. It also issues a warning if a certificate expires soon or is already invalid. If a snap handles cryptographic material, it can use this centralized service following the instructions:


First, the snap has to provide the content interface "package-certificates". Via this interface, the certificate manager can add and access certificates and keys of a specific snap. Thus, write permissions are required.

!!! Example

    ```yaml
    name: rexroth-solutions
    [...]
    slots: 
    [...]
    package-certificates:
        interface: content
        content: package-certificates
        source:
        write:
        - $SNAP_COMMON/package-certificates/${SNAPCRAFT_PROJECT_NAME}
    [...]
    ```

#### Certificate Stores

The certificate store block defines whether a snap service has to handle cryptographic keys or certificates. A snap should have different certificate stores for different services. These stores are defined in the package-assets file (see following code snippet)

!!! example
    ```json
    "certificatestores": [
          {
              "id": "examplestore",
              "title": "Example Store",
              "description": "This is only an example store for documentation purposes.",
              "scopesR":[
                "example.permission.r"
              ],
              "scopesRWX":[
                "example.permission.rwx"
              ]
          }
      ],
    ```

* **id (mandatory):** Use a unique ID, as it is used to identify the store via the REST interface.
  * opcua
  * vpnmanager
  * ...
* **title (mandatory):** Name used in the front end.
  * OPCUA
  * VPN Manager
  * ...
* **description:** Optional and displayed in the front end. To describe the application and provide some more information. 
* **scopesR:** Optional list of scopes that allow the user to have read access to this certificate store.
  * rexroth-solutions.web.all.r
  * example.permission.r
  * ...
* **scopesRWX:** Optional list of scopes that allow the user to have full access to this certificate store.
  * rexroth-solutions.web.all.rw
  * example.permission.rwx
  * ...

The following predefined folder structure applies to every certificate store:

!!! hint
    SNAPCRAFT_PROJECT_NAME has to be replaced with the technical app name!



!!! example

    ``` title="/$SNAP_COMMON/package-certificates/{SNAPCRAFT_PROJECT_NAME}/"
    ./application1
        /ca
        /own
            /private
            /certs
        /rejected
            /keys
            /certs
        /trusted
            /keys
            /certs
    ./application2
        /ca
        ...
    ```

!!! hint
    You can store your keys using a [TPM](https://en.wikipedia.org/wiki/Trusted_Platform_Module).

### Required apps (optional)

In specific scenarios (e.g. Brandlabeling (Theming App) or as ctrlX OS Partner) you might want to prohibit the uninstallation and deactivation of your app. To do so, you can set `"required": true` in your package-manifest. This option is only allowed in specific scenarios and requires a justification as part of validation of the app.

!!! example

    ```json
    {
      "required": true
    }
    ```

### Example package-manifest.json

??? example

    ```json
    {
        "version": "1.0.0",
        "id": "rexroth-solutions",
        "certificatestores": [
            {
                "id": "examplestore",
                "title": "Example Store",
                "description": "This is only an example store for documentation purposes."
            }
        ],
        "services": {
            "proxyMapping": [
                {
                    "name": "rexroth-solutions.web",
                    "url": "/rexroth-solutions",
                    "binding": ":5000",
                    "restricted": [
                        "/rexroth-solutions/api/v1.0"
                    ]
                }
            ]
        },
        "scopes-declaration": [
            {
                "identifier": "rexroth-solutions.web",
                "name": "Solutions Scopes",
                "description": "Scopes for the Solutions resources",
                "scopes": [
                    {
                        "identifier": "rexroth-solutions.web.all.rw",
                        "name": "Manage Solutions",
                        "description": "Manage and modify all solutions"
                    },
                    {
                        "identifier": "rexroth-solutions.web.all.r",
                        "name": "View solutions",
                        "description": "View (but not modify) solutions"
                    }
                ]
            }
        ],
        "menus": {
            "sidebar": [
                {
                    "id": "solutions",
                    "title": "Solutions",
                    "icon": "bosch-ic-solutions",
                    "permissions": [
                        "solutions.r",
                        "solutions.rw"
                    ],
                    "items": [
                        {
                            "id": "solution-home",
                            "title": "Solutions Home",
                            "link": "/solutions",
                            "permissions": [
                                "solutions.r",
                                "solutions.rw"
                            ]
                        },
                        {
                            "id": "solution-targets",
                            "title": "Targets",
                            "link": "/solutions/targets",
                            "permissions": [
                                "solutions.r",
                                "solutions.rw"
                            ]
                        }
                    ]
                }
            ],
            "settings": [
                {
                    "id": "solution-settings",
                    "title": "Solution Settings",
                    "icon": "bosch-ic-gear",
                    "link": "/solutions/settings",
                    "permissions": [
                        "solutions.rw"
                    ]
                }
            ],
            "overview": [
                {
                    "id": "solution-overview",
                    "title": "Solutions",
                    "icon": "bosch-ic-solutions",
                    "link": "/solutions",
                    "permissions": [
                        "solutions.r",
                        "solutions.rw"
                    ]
                }
            ]
        }
    }
    ```
## The language files

Language files are used to translate text sections into different languages. Bosch Rexroth Apps are shipped with english and german language files. Currently the multi language system (MLS) supports translations for the content of the [package manifest](#the-package-manifest) texts which are organized in the `./package-assets/i18n` folder of the app.

### Translation of the Package Manifest

One aspect of the MLS is the translation of the text fields provided by the package manifest, e.g. menu items, descriptions etc. To enable the app developer to provide translations for these fields a new property `i18n` was introduced. This property behaves as a tag for the parent object that requires translation for its childs. The value is defining the name of the tag in *dot notation*.

Additionally you have to provide the language files for the package-manifest in the `./package-assets/i18n` folder. 

The package-manifest language file name should follow the pattern:

     <snap name>.package-manifest.<language tag>.json

The _snap name_ is the _name_ of your app, configured by the _snapcraft.yaml_.

In case of an app "my-app" that provides german translations, this would be `my-app.package-manifest.de.json`. It is recommended to provide a language file for english and german. The structure inside the language file must reflect the tags defined including the propery you would like to translate. You can either use dot, bracket or mixed notation in the file. Arrays are not supported.

!!! Example

    ```json title="myapp.package-manifest.json"
        [...]
        "menus": {
            "sidebar": [
                {
                "id": "_myapp",
                "title": "My App",
                "description": "A cool app, you should not miss",
                "icon": "Bosch-Ic-home-outline",
                "link": "/myapp/home",
                "i18n": "sidebar.myapp"
                }
            ],
        },
        [...]
    ```

    ```json title="i18n/myapp.package-manifest.de.json (dot notation)"
    {
        "sidebar.myapp.title": "Meine App",
        "sidebar.myapp.description": "Eine coole App, die du nicht verpassen solltest"
    }
    ```

    ```json title="i18n/myapp.package-manifest.de.json (bracket notation)"
    {
        "sidebar": {
            "myapp": {
                "title": : "Meine App",
                "description": "Eine coole App, die du nicht verpassen solltest"
            }
        }
    }
    ```

Basically you can translate every aspect of the package-manifest this way.