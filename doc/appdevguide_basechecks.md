This document is part of the Bosch Rexroth ctrlX CORE App Development Guideline, and describes which basic aspects are checked in the Validation Process.

Please refer to the App Development Guide for further information.

The content of this document may be subject of change in further versions of the Validation Process.

****

# 1. snapcraft.yaml

## 1.1 Checked content

| **Item** | **Criteria** |
| --- | --- |
| **title** | is unique and length is between 3 and 40  characters |
| **name**  | matches with the snap name (technical app name) |
| **version** | follows semantic versioning (`MAJOR.MINOR.PATCH`) and does not exceed 32 characters |
| **description** | provides a short description of the App and is at least 16 characters long |
| **degree of isolation** | is set to `confinement = "strict"` |
| **grade** | is set to `grade = "stable"` |
| **restart delay** | ist set to `5` or higher |
| **plugs** | list of interfaces consumed by the snap |
| **slots** | list of interfaces provided by the snap (optional)|
| **package assets** | to exchange information |
| **package-certificates** | certificates and keys of the snap (optional) |

## 1.2 Additional notes

- **Schema for snapcraft.yaml:** <https://github.com/snapcore/snapcraft/blob/master/schema/snapcraft.json>
- **Schema for technical app  name:** "ctrlx-{company name}-{app name}-{app version}_arm64/amd64.snap"
- **ctrlX Device Portal: App name in Private Repository:** If the App is provided in a Private Repository for the ctrlX Device Portal, the Name in the Artifact Description in the Repository should be identical to the Name in snapcraft.yaml. Please see <https://docs.automation.boschrexroth.com/doc/2056862327/ctrlx-device-portal/latest/en/> for additional information about the ctrlX Device Portal and the Private Repository.

****

# 2. package-manifest.json

## 2.1 Checked content

| **Item** | **Criteria** |
| --- | --- |
| **licenses** | list of all used licenses (`licenses.name, .title, .description .required`) |
| **certificatestores** | snap service to handle keys and certificates (optional) |
| **menus** | optional: entries for sidebar integration (if use, the entries must be unique) |

For each given Proxy Mapping `services.proxyMapping[i]`:

| **Item** | **Criteria** |
| --- | --- |
| **name** | is only lowercase, contains `.web` and is unique in the list |
| **url** | is only lowercase and starts with `/` |
| **binding** | is valid / not empty |

## 2.3 Additional notes

- the *package-manifest.json* file must provide service entries for all services which are used in in the *portlist-description.json* file (proxy.service-name, see 3.)
- the *package-manifest.json* file must provide service entries for all services which are used in in the *unixsocket-description.json* file (socket name and accessibility, see 4.)

****

# 3. portlist-description.json

## 3.3 Checked content

| **Item** | **Criteria** |
| --- | --- |
| **id** | is available and matches to the technical app name (see also **name** entry in *snapcraft.yaml*) |

For each given port in the description (`ports[i]`):

| **Item** | **Criteria** |
| --- | --- |
| **number**|  has 3 to 5 digits and exists only once in the list|
| **purpose**| has at least 16 characters|
| **application-protocol**| has at least 3 characters|
| **binding**| (Network Binding Interface) is either localhost or IP|
| **routing** |is either internal or external|
| **service-name** |starts with `ctrlx-` or `bosch-`, is lowercase and exists only once in the list|
| **default-state**| is either `open` or `closed`|

## 3.4 Additional Notes

- **Number of open ports**: For security reasons, the number of open ports should be limited to 3
- **Unwanted direct external access**: The following configuration should be avoided:
  - `"protocol = HTTP or HTTPS"`, `"binding = IP"` and `"routing = external"`

- **Wrong configuration**: these configurations are considered as mismatch:
  - `"protocol not HTTP or HTTPS"`, `"binding = IP"` and `"routing = internal"`
  - `"protocol not HTTP or HTTPS"`, `"binding = localhost"` and `"routing = external"`

- **Port conflicts with reserved ports**:
  - Ports that are listed in the [Reserved interfaces and ports](appdevguide_reserved-interfaces.md) will generate a vakidation error

****

# 4. unix-socket-description.json

## 4.1 Checked content

For each given socket `(sockets[i])`

| **Item** | **Criteria** |
| --- | --- |
| **name** |is only lowercase, end with `.sock` or `.socket` and exists only once in the list|
| **purpose**| is at least 16 characters long|
| **accessibility** |is either `internal` or `external`|

## 4.2 Additional notes

- only web sockets (name contains `.web`) can be used for external accessibility

****

# 5. slotplug-description.json

## 5.1 Checked content

For each given slot `(slots[i])` and plug `(plugs[i])`

| **Item** | **Criteria** |
| --- | --- |
| **name** |is only lowercase|
| **purpose** |is at least 16 characters long|

## 5.2 Additional notes

- both for slots an plugs, the max number of entries should be 8
- All slots and plugs in the *slotplug-description.json* must be available in the *snapcraft.yaml* file also, and vice versa

# 6. Scopes (${snapname}.scopes.json)

## 6.1 Checked content

| **Item** | **Criteria** |
| --- | --- |
|**id** | must be assigned (optional) |
| **required-permissions** | must match with the used permissions (optional) |

****
**Copyright**
© Bosch Rexroth AG 2024 -
This document, as well as the data, specifications and other information set forth in it, are the exclusive property of Bosch
