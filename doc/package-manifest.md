### Basics

An app can bring their own menu entries, use the reverse proxy infrastructure including permissions and use the solution for storage. This document describes the different integration points and how they work.

Essential for the integration points to work is the package-manifest. Beside the snapcraft.yaml, it has to be provided by the app to integrate probably into the ctrlX CORE ecosystem. Here is an example of a package-manifest.json:


??? example

    ```json
    {
        "version": "1.0.0",
        "id": "rexroth-solutions",
        "certificatestores": [
            {
                "id": "examplestore",
                "title": "Example Store",
                "description": "This is only an example store for documentation purpose."
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


### Providing the package-manifest

The manifest file has to be provided to app.deviceadmin by the third-party app, therefor a data exchange between both apps has to be established. To exchange the information, the content interface with the name "package-assets" is used. This content interface works inverted to the "normal" content interface. Instead of the slot is providing data that is used by the plug. The slot provides a folder where the plugs directories are mounted in. This principle is called "spool mode" by canonical.

The following example shows you step-by-step, what you have to do.

1. Inside the root of your snap project, create a folder "configs/package-assets".

2. Place your package-manifest into this folder and name it using the pattern `snap-name`.package-manifest.json, e.g. rexroth-solutions.package-manifest.json.

3. Update your snapcraft.yaml
    * Add a new part which dumps the content of the "configs" directory into your $SNAP folder.

    * Add the slot "package-assets" to your slots configuration


snapcraft.yaml:

```yaml
name: rexroth-solutions
[...]
parts:
[...]
  configs:
    plugin: dump
    source: ./configs
[...]
slots: 
[...]
  package-assets:
    interface: content
    content: package-assets
    source:
      read:
      - $SNAP/package-assets/${SNAPCRAFT_PROJECT_NAME}
[...]
```

### Menus

An app can integrate into the ctrlX CORE navigation by defining menu entries. There are three possible integration points:

* Sidebar: to allow navigation to your apps main functionality
* Settings: to provide settings dialogs and pages
* Home Overview: to provide a custom tile for your application
  
![Integration](./samples-go/hello.webserver/docs/images/integration.png)

### Declare menu items in package-manifest

Add a "menus" element to your snap’s package manifest. The "menus" element is defined as an object that comprises three elements:

* sidebar: an array of items to be integrated into the main menu
* settings: an array of items to be integrated into the settings menu
* system: an array of items to be integrated into the system menu
* overview: an array of items to be integrated into the app overview

An item is defined as an object that comprises the following elements:

* id: a string serving as the unique item identifier; used for ordering items (required)
* title: a string serving as the item’s display title (required)
* description: a string providing an additional description (optional and only used by overview items)
* icon: a string representing the name of the item’s icon (optional)
* link: a string representing the route associated to this item, including the application’s base href (required if no "items" are provided)
* target: a string that specifies where the link should be shown. Use "_blank" or a specific name to open the link in a new tab (recommended for 3rd-party apps)
* items: an array of sub-items of this item (required if no link is provided; currently, only supported for main menu items and restricted to one level of depth)
* permissions: an array of scope identifiers representing the permissions required to enable the item (optional)

??? example 

    ```json
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
        "system": [
          {
            "id": "reboot",
            "title": "Reboot",
            "icon": "bosch-ic-gear",
            "link": "/reboot-manager"
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
      }
    [...]
    ```


When the third-party app is using a dedicated port and is not using our reverse proxy infrastructure, you can use the template variable "${hostname}" to let the ctrlX CORE replace it dynamically. E.g.:

!!! note

    ```json
    [...]
              {
                "id": "node-red",
                "title": "Node-RED",
                "link": "http://${hostname}:1880/",
              }
    [...]
    ```

### Reverse Proxy

The Reverse Proxy handles all **external** web-based (HTTP/HTTPS) requests to the device. The Reverse Proxy provides the following functionality:

* **Automatic HTTPS:** By default, all data exchanged between client and server is encrypted.
* **Token Validation:** By default, only authenticated users may access resources on the device (except the login page). Requests with invalid tokens (lifetime expired, invalid signature) are rejected.

The Proxy Mapping provides the configuration, which is required by the Reverse Proxy to redirect requests for a specific URL to a web server running on the device.

**Proxy Mapping**


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
  }
[...]
}
```

Providing a proxy configuration is optional in some cases (e.g. if your snap does not provide a web service). If a proxy configuration is supplied, the following parameters are valid:

* **name (mandatory):** Name of the web service.  The format is: `<id>.<service>`The name has to be unique. Examples are:
  * `rexroth-solutions.web`
  * `rexroth-hmi.web`
  * ...
* **url (mandatory):** URL which is provided by the Reverse Proxy. When a client tries to access this URL, the Reverse Proxy transparently redirects the request to the web server, this means your webserver has to be configured to listen to e.g. "/cool-app" and not "/". Must not conflict with other web services URLs. Examples are:
  * /solutions
  * /hmi
  * ...
* **binding (mandatory):** Identifier of the resource to which the reverse proxy redirects the requests. The resource may be a port or a unix socket. Using a unix socket is highly recommended. Examples are:
  * :5000
  * 192.168.1.17:5000
  * "unix://{$SNAP_DATA}/package-run/rexroth-solutions/rexroth-solutions.web.sock"
* **restricted:** Restricted URL. URLs defined here are accessible for authenticated clients only. One important example are the API URL(s) - those URLs should be restricted in the majority of the use cases. Examples:
 "/rexroth-solutions/api/v1.0"

!!! info
    **Remark:** In case a snap provides multiple web servers, multiple entries of ProxyMapping are allowed.

#### Binding

**Unix sockets (recommended)**

To use unix sockets the web server of your application has to bind against a [Unix Domain Socket](https://en.wikipedia.org/wiki/Unix_domain_socket). Most of web server frameworks already support this. The benefit of unix sockets is the additional security (file access can be restricted via file permissions) and the lower attack surface on network level (the service cannot be reached externally). To enable your snap and to provide the reverse proxy access to your unix socket, you have to use the content interface slot "package-run". It uses the same mechanism as the "package-assets" interface.

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

In the example mentioned above, your "unix socket file" should be created under `{$SNAP_DATA}/package-run/{$SNAP_NAME}/web.sock`. Please ensure, that the file is completely managed by your application logic and, other than a network socket, has to be deleted after unbind. Also consider deleting it before binding your web server on start up.

Now, you need to publish the information about the socket file using the package-manifest using the ProxyMapping described above. The relevant part is the "binding" section below. You have to replace

!!! warning
    The path length of a unix socket is limited to 108 characters. Therefore please ensure, that the resolved path, e.g /var/snap/third-party-snap/current/==package-run/third-party-snap/third-party-snap.web.sock==, respects that limit and the highlighted part has a maximal length of 50 characters. If necessary, you can shorten the path by replacing "package-run" with "run" and shorting the file name from "third-party-snap.web.sock" to "web.sock". See the following example:


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

**Localhost (not recommended)**

Binding to localhost should only be chosen, when a Unix Domain Socket (see above) cannot be used. It requires that your app binds to a dedicated port that is not used by other apps.



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

In case access to specific resources shall be restricted, scopes have to be defined. By using the Identity Management, an administrator is able to configure which users/groups are allowed to access specific resources. The following example shows two simple scopes (read/write all settings, read settings). The web server has to check in each routine if the provided token includes the scope.

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

**Remark: Restrictions apply regarding the naming of the scopes, identifiers, etc. (please see below)**

The main identifier of the scope must follow this schema by convention: 

`<id>.<service>`

* id: Identifier of the package. Must by convention correspond to the name of the snap
  * Example: rexroth-solutions, rexroth-vpnmanager, ...
* service: Identifier of the service
  * Example: web

Some examples:

* rexroth-solutions.web
* rexroth-vpnmanager.web

The identifier of each specific scope must follow this schema by convention: 

`<id>.<service>.<scope_name>.<access>`

* id : Identifier of the package. Must by convention correspond to the name of the snap
  * Example: rexroth-solutions, rexroth-vpnmanager, ...
* service: Identifier of the service
  * Example: web
* scope_name: Name of the specific scope
  * Example: solutions, connections, configurations, users, ...
* access: Type of access to the resource which is represented by this scope. Supported are:
  * r (read-only): Allows read access to specific resources
  * w (write-only): Allows write-only access to specific resources
  * rw (read/write): Allows read and write access to specific resources
  * x (execute): Execute a specific action which is represented by a resource

Some examples:

* rexroth-vpnmanager.web.shortcuts.r: Read access to VPN shortcuts
* rexroth-vpnmanager.web.shortcuts.rw: Read and create VPN connection shortcuts
* rexroth-deviceadmin.web.own_password.w: Update (but not read) the password of the user who is currently logged in
* rexroth-vpnmanager.web.shortcuts.x: Start / stop VPN connections via existing starters (shortcuts)

#### Enforcing permissions in your service

The web server has to enforce the defined permissions. The following pseudo code snippet shows a light-weight example in which the function ListAddresses checks whether the client has the permission to execute the function. The scope "rexroth-device.all.rwx" is a special scope, which reflects an administrative access permission (see below). 

```golang
func ListAddresses(w http.ResponseWriter, r *http.Request) {
     
    // Define valid scopes for resource access
    scope := []string{"networkmanager.all.r", "networkmanager.all.rw", "rexroth-device.all.rwx"}
 
 
    // Extract scopes from http request and compare to valid scopes
    // Return when permissions are not sufficient
    if ok, _ := checkPermissions(r, scope); !ok {
        errors.ErrUnauthorized.Write(w)
        return
    }
```

#### Admin Scope

The Identity Management by default provides a special admin scope ("rexroth-device.all.rwx"), which should only be assigned to admin users.  It is recommended - but not mandatory - to grant users with this permission full access to your web application. This makes the handling of administrator accounts easier.

The following snippet shows the definition of the admin scope. You can see (and assign) the permission to users and/or groups.

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

### Certificate Management (optional)

The certificate managers handles cryptographic material for all applications on the device. The certificate manager has the ability to add/delete/list certificates or keys to/from a specific application. It also warns the user if a certificate expires soon or is already invalid. If a snap handles cryptographic material it can use this centralized service by following the instructions:


First, the snap has to provide the content interface "package-certificates". Via this interface the certificate manager is able to add and access certificates and keys of a specific snap. This is why write permissions are necessary.


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

The certificate store block defines if a snap service has to handle with cryptographic keys or certificates. A snap should have different certificate stores for different services. These stores are defined within the package-assets-file (see following code snipped)

!!! example
    ```json
    "certificatestores": [
          {
              "id": "examplestore",
              "title": "Example Store",
              "description": "This is only an example store for documentation purpose."
          }
      ],
    ```

* **id (mandatory):** This id should be unique because it is used to identify the store over the REST-interface.
  * opcua
  * vpnmanager
  * ...
* **title (mandatory):** This name is used in the frontend.
  * OPCUA
  * VPN-Manager
  * ...
* **description:** The description is optional and displayed in the fronted. Here you can describe the application and can give the user some more information. 

Every certificate store needs the following, predefined folder structure:

!!! hint
    SNAPCRAFT_PROJECT_NAME has to been replaced with the real name here!

/$SNAP_COMMON/package-certificates/{SNAPCRAFT_PROJECT_NAME}/

!!! example
    * /application1
      * /ca
      * /own
        * /private
        * /certs
      * /rejected
        * /keys
        * /certs
      * /trusted
        * /keys
        * /certs
    * /application2
      * /ca
      * ...

!!! hint
    You can store your keys with the help of a [TPM](./tpm2.md).

#### Integration possibilities

Third-Party Application that brings its own web server, has got a possibility to integrate in ctrlX CORE user interface. 

After this integration, a Third-Party Application will be opened in a separate tab within the browser.

The **recommended integration** with ctrlX CORE is to use **package-manifest.json** file.

Within this file, you can specify the section for the menus and the sidebar. 

The important part is the assignment of the **bearer token** for the external authentication.

The pattern for this file is shown below.

    ```package-manifest.json
	...
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
          },...
		]
	  }]
    [...]
    ```
