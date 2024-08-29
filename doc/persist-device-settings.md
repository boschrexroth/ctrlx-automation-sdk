### **Table of content**

[Introduction](#introduction)

[Using REST API endpoints](#rest)

[Using Ubuntu’s snapshot mechanism](#snapshot)

## Introduction<a name="introduction"></a>

Apps can integrate into the ctrlX Setup functionality so that their device-specific app settings are saved
or restored when a backup is created or restored. There are two ways of integration:

| Using REST API endpoints                                                     | Using Ubuntu’s snapshot mechanism                                                                                  |
|:-----------------------------------------------------------------------------|:-------------------------------------------------------------------------------------------------------------------|
| Fine-grained control with respect to the settings format and content         | Limited control with respect to the settings format and content                                                    |
| App settings can be saved or restored in any system state                    | The app will be disabled before taking or restoring its snapshot, which requires the system to be in SERVICE state |
| Secrets can be stored as encrypted files                                     | Snapshot contents are stored unencrypted so that they must not include secrets                                     |
| Unchanged settings are skipped on restore                                    | Snapshot contents are always completely restored                                                                   |
| Requires a web server and the implementation of API endpoints | Minimal implementation effort                                                                                      |

## Using REST API endpoints<a name="rest"></a>

For using this integration approach, an app must provide REST API endpoints that allow to access its settings.
Settings of apps that are not accessible through settings endpoints will not be included in a backup.

### Register app settings

An app declares the settings to be included into a backup in the "apps"/"rexroth-setup" section of its package manifest.

Each setting consists of a setting name, a description, a URL used to read and update the settings value, and a value
type:

- **name** (required): Name of the setting. Must be unique within the app’s settings list.
  Serves as key in the ctrlx-backup.json file. Note that the name "$path" is reserved.
- **description**: Short localized description of the setting.
- **url** (required): URL used to read (GET) and update (PUT) the setting. A URL starting
  with a slash is interpreted to be relative to `https://localhost`.
- **type**: One of
  - **"text"** (default): The value is saved as a quoted string.
  - **"json"**: The value is saved in JSON object format (see "Merge operation for JSON-formatted settings").
  - **"file"**: The value is persisted as a file.
  - **"encryptedFile"**: The value is persisted as an encrypted file if the backup is
created with a password.

**package-manifest.json:**

```json
{
  "apps": {
    "rexroth-setup": {
      "settings": [
        {
          "name": "foo",
          "description": "Description of foo",
          "url": "/myapp/settings/foo",
          "type": "text"
        },
        {
          "name": "bar",
          "description": "Description of bar",
          "url": "/myapp/settings/bar",
          "type": "json"
        },
        {
          "name": "baz",
          "description": "Description of baz",
          "url": "/myapp/settings/baz",
          "type": "file"
        },
        {
          "name": "secret",
          "description": "Description of secret",
          "url": "/myapp/settings/secret",
          "type": "encryptedFile"
        }
      ]
    }
  }
}
```

The Setup app calls the URLs with GET or PUT to read or update the settings when a backup is created or restored.

Creating a backup with the above setting declarations might, e.g., result in the following `ctrlx-backup.json` output. The
respective files (`baz.bin`, `secret.bin.aes`) would be included in the enclosing `ctrlx-backup.zip` archive.

**ctrlx-backup.json:**

```json
{
  "apps": {
    "myApp" : {
      "settings": {
        "foo": "some value",
        "bar": {
          "enabled": true,
          "title": "my title"
        },
        "baz": {
          "$path":"apps/myApp/settings/baz.bin"
        },
        "secret": {
          "$path":"apps/myApp/settings/secret.bin.aes"
        }
      }
    }
  }
}
```

### Implement settings endpoints

Apps must consider the following conditions and constraints in their implementations:

- App settings are applied after apps have been updated and device admin settings (users, scopes, licenses,
...) have been applied.
- The order in which requests are sent to read or update the settings is undefined.
- Persisted settings must be usable with newer versions of the app. A newer app version may introduce new settings and
properties which must be tolerated (ignored) by older app versions. Incompatible changes (e.g. changing the meaning of
enum values) between app versions must be avoided.
- Apps using the settings type "json" must implement the special semantics described in the following
section.

#### Merging JSON-formatted settings

The restore operation allows fine-grained modifications by merging the backup content with the current state of the device.
This way, new properties may be added to JSON objects (new users may, e.g., be added to the existing ones on the device).
Conversely, properties can be removed by setting their values to `null`.

Apps can use the merge feature by providing their property settings with type "json" and by implementing the following
behavior:

| Use case                                                                                                    | Example                                      | Description                             |
|:------------------------------------------------------------------------------------------------------------|:---------------------------------------------|:----------------------------------------|
| A non-existing property in the JSON structure will leave existing properties on the device untouched.       | `"users": {}`                                | Does not change users on the device     |
| The value of a property in the JSON structure will update the corresponding property value on the device.   | `"users": {"john": {"passwordHash": "..."}}` | Updates the passwordHash of user "john" |
| A new property in the JSON structure will be added on the device.                                           | `"users": {"jane": {"passwordHash": "..."}}` | Adds new user "jane"                    |
| A property with a `null` value in the JSON structure will remove the corresponding property on the device.  | `"users": {"john": null}`                    | Deletes user "john"                     |

#### Success behavior

- **GET**: Apps must respond with status code 200 if the request has succeeded. The response body must contain the
setting value in the following format:
  - "text" => text/plain
  - "json" => application/json
  - "file"/"encryptedFile" => application/octet-stream
- **PUT**: Apps must apply the setting value contained in the request body and respond with status code 204 and an empty
response body if the request has succeeded.

#### Failure behavior

- Apps must respond with an error status code (4xx or 5xx) if a GET or PUT request has failed.
- The response body must contain a JSON object describing the problem by providing an appropriate status code and title
(cf. example and schema below).
- The Setup app reports the problem response of a failed request as an error in the Logbook and via Web UI. Processing
is continued with the next setting.

##### Problem response body (example)

```json
{
  "status": 400,
  "title": "Cannot parse value."
}
```

##### Problem schema

```yaml
Problem:
  type: object
  x-version: '1.0.0'
  description: |
    This document defines a "problem detail" as a way
    to carry machine-readable details of errors in a
    HTTP response to avoid the need to define new error
    response formats for HTTP APIs.
  required:
    - type
    - title
  properties:
    type:
      type: string
      format: uri
      description: |
        A URI reference [RFC3986] that identifies the problem type.
        This specification encourages that, when de-referenced,
        it provide human-readable documentation for the problem type
        (e.g., using HTML [W3C.REC-html5-20141028]). When this member
        is not present, its value is assumed to be "about:blank".
      default: 'about:blank'
      example: 'https://example.com/probs/out-of-credit'
    title:
      type: string
      description: |
        A short, human-readable summary of the problem type.
        It SHOULD NOT change from occurrence to occurrence of the problem,
        except for purposes of localization (e.g., using proactive content
        negotiation; see [RFC7231], Section 3.4).
        If the response is a diagnostics message, it contains
        the text associated with the mainDiagnosisCode.
      example: You do not have enough credit.
    status:
      type: integer
      format: int32
      description: |
        The HTTP status code ([RFC7231], Section 6) generated by the
        origin server for this occurrence of the problem.
      example: 403
    detail:
      type: string
      description: |
        A human-readable explanation specific to this occurrence of the problem.
        If the response is a diagnostics message, it contains the text associated with
        the detailedDiagnosisCode.
      example: Your current balance is 30, but that costs 50.
    instance:
      type: string
      description: |
        A URI reference that identifies the specific occurrence of the problem.
        It may or may not yield further information if de-referenced.
      example: /account/12345/msgs/abc
    code:
      type: string
      deprecated: true
      description: |
        An error code issued by the system that caused the original problem.
        This code can be used to track down the root cause of the error.
      example: F360001
    mainDiagnosisCode:
      type: string
      description: |
        The main diagnosis code is issued by the system that caused the problem.
        This code can be used to track down the root cause and source of the error.
        It can be used to search in the documentation for a solution.
        It SHOULD NOT change from occurrence to occurrence of the same problem.
      example: 080F0100
    detailedDiagnosisCode:
      type: string
      description: |
        The detailed diagnosis code issued by the system that caused the problem.
        This code can be used to track down the detailed cause and source of the error.
        It can be used to search in the documentation for a solution.
        The detailed diagnosis code gives additional information about the cause of the error.
        It SHOULD NOT change from occurrence to occurrence of the same problem.
      example: '00666001'
    dynamicDescription:
      type: string
      description: |
        A dynamic description gives detailed information about the occurrence of a problem.
        It can change between different occurrences of the same error.
      example: value = -4.5
    severity:
      type: string
      description: 'Severity of a problem as defined RFC5424 of the Syslog standard, see https://tools.ietf.org/html/rfc5424'
      example: Error
      enum:
        - Emergency
        - Alert
        - Critical
        - Error
        - Warning
        - Notice
        - Informational
        - Debug
    links:
      type: array
      description: |
        Collection of links to fix the problem. E.g. links to online user manual,
        to an online community (using tags) or a developer chat (e.g. Slack).
      items:
        type: string
        format: uri
        example: https://stackoverflow.com/questions/tagged/boschrexroth+controlx+f360001
    moreInfo:
      type: object
      description: |
        The moreInfo field allows you to add additional information that might be necessary to debug or trace down the cause of an error.
        It might also contain information what is needed to resolve the problem.
        There are loosely specified fields for specific problems that can be used in these situations, e.g.:
        - On a 403 Forbidden error: The moreInfo can contain a field "requiredPermissions" with the required permissions seperated by comma
        - On a 402 Payment required: The moreInfo can contain a field "requiredLicenses" with the required licenses seperated by comma
      additionalProperties: true
      example:
        trace: ExampleClass.cpp#452 Null pointer exception
        class: ExampleClass
    cause:
      type: array
      description: |
        Sometimes a problem might be caused by problems that occur in other components or are the root cause of a problem. In this
        case the related problems can be added as a cause. Since it might be possible that multiple components have problems on a single
        action, cause is an array. E.g. Loading failed because several components where not able to load the configuration.
      items:
        $ref: "#/Problem"
```

## Using Ubuntu’s snapshot mechanism<a name="snapshot"></a>

An app may use Ubuntu’s snapshot mechanism to save and restore its settings, cf. <https://snapcraft.io/docs/snapshots>.
While this minimizes the implementation effort, the following issues should be considered:

- A snapshot typically contains much more files than should be part of the apps settings, e.g. log files, cached files, etc.  In order to keep backup file sizes manageable, apps should exclude irrelevant content from their snapshots if possible (cf. <https://forum.snapcraft.io/t/excluding-data-from-snapshots/33643>).
- Secrets are saved unencrypted in the snapshot and the backup file, respectively.
- Taking a snapshot fails if a file is locked or modified while taking the snapshot. Therefore, the Setup app disables apps before taking a snapshot, which requires that the system is in SERVICE state. Apps must be prepared to consistently save their state on "disable" events. When the snapshot has been taken, the Setup app enables the app again.
- Before restoring a snapshot, the Setup app disables the app. After the snapshot has been restored, the Setup app enables the app so that the app can initialize itself with the restored data. As the Setup app cannot find out if a snapshot contains changed settings, snapshots are always completely restored.

The following entry in an app’s package manifest enables the use of Ubuntu’s snapshot mechanism for saving and restoring
its settings:

**package-manifest.json:**

```json
{
  "apps": {
    "rexroth-setup": {
      "settings": [
        {
          "type": "snapshot"
        }
      ]
    }
  }
}
```

Creating a backup with the above settings declaration would result in the following `ctrlx-backup.json` output. The
respective snapshot file (`app.snapshot`) would be included in the enclosing `ctrlx-backup.zip` archive.

**ctrlx-backup.json:**

```json
{
  "apps": {
    "myApp" : {
      "settings": {
        "$path": "apps/myApp/settings/app.snapshot"
      }
    }
  }
}
```
