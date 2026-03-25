## OEM App Signing

As an OEM intending to sell machines or appliances running ctrlX OS, you will likely have custom-developed apps for these specific use cases. In these cases, instead of relying on Bosch Rexroth AG to sign your apps, OEM App Signing enables you to sign your own apps using your own private key and signed OEM certificate issued by Bosch Rexroth AG.

### Applying for OEM App Signing

OEM App signing is a feature that you need to request and sign a contract for.

* What you need: Private signing key
* What you receive: "OEM App signing certificate" issued for your private key

!!! important
    You are responsible for the private signing key. You MUST ensure it is protected according to the state-of-the-art. It is strongly advised to use HSMs for this. In case of loss or leak of the private key, Bosch Rexroth has all rights to revoke the OEM App signing certificate issued for you. As a consequence, your apps might not run any more with future updates of ctrlX OS.

Procedure:
1. Sign OEM App signing contract
2. Generate your private signing key
3. Create a Certificate Signing Request (CSR) for this key. The CSR needs to contain at least the "Organization" field for the subject.
4. Send this CSR to Bosch Rexroth AG
5. Bosch Rexroth AG signs the CSR, and sends the OEM App signing ceritifcate back to you
6. Sign apps with your private key and OEM App signing ceritifcate

If you want to apply for OEM App signing, get in contact with your field application engineer or sales contact.

### Installing OEM signed apps

Your signed apps are not accepted for installation by default. You need to upload your "OEM App signing certificate" to your ctrlX OS devices. This certificate needs to be uploaded into the trusted category of the device's certificate store "App signature validation certificates" (id: appvalidationcerts) before installing an OEM-signed app. This should be done during the provisioning process of the device.

To upload the "OEM App signing certificate", you can either use the "Certificates & Keys" settings in the Web UI or the [certificates REST API](https://boschrexroth.github.io/rest-api-description/ctrlx-automation/ctrlx-core/).

If your "OEM App signing certificate" is uploaded to the ctrlX OS device, your signed apps can be installed just like every other app.

### App structure and signatures

Your app needs to comply with the [Package Assets](package-assets.md) definition. Additionally, you need to provide a `.signature` file for each `.snap` file inside of the app. The `.app` file must be structured like this:

```
<app-name>_<app-version>.app
└──public
   └──snaps
      └──<arch>
         └──release
            ├──<app-name>_<app-version>.signature
            └──<app-name>_<app-version>.snap
```

Example:

```
myapp_1.4.3.app
└──public
   └──snaps
      ├──arm64
      │  └──release
      │     ├──myapp_1.4.3.signature
      │     └──myapp_1.4.3.snap
      └──amd64
         └──release
            ├──myapp_1.4.3.signature
            └──myapp_1.4.3.snap
```

### App signatures

App signatures often consist of multiple signature blocks, each serving a specific purpose. While they are part of a single file, they are processed and verified individually. Two types of signature declarations are currently supported.```

Structure of the .signature files:

```
<app-revision>
<empty line>
<app-revision signature>
<empty line>
<app-privileges>
<empty line>
<app-privileges signature>
```

Whereas:

- app-revision signature: sha384 digest of \<app-revision\> signed by OEM, base64 encoded
- app-privileges signature: sha384 digest of \<app-privileges\> signed by Bosch Rexroth, base64 encoded

#### App revision

The app-revision declaration proves the origin and authenticity of the snap-files inside your app.

Structure of app-revision:

```
type: app-revision
app-name: <app-name>
publisher: <common name from the OEM app signing certificate you received>
organization: <your company name from field "organization" of your certificate>
timestamp: <current time stamp>
snap-digest: <sha384 digest of .snap file>
```

#### App privileges

Security related interfaces are so-called [Reserved interfaces](appdevguide_reserved-interfaces.md). These are denied or restricted by default. If you want to use such an interface inside your app, you need to submit a formal and reasoned request. The request will be reviewed. If the request is granted, you receive the app-privileges declaration with an according signature for your specific app.

For your app-privileges requests, get in touch with your field application engineer or sales contact. Use the following app-privileges structure for your request:

```
type: app-privileges
app-name: <app-name>
publisher: <common name from the OEM app signing certificate you received>
timestamp: <current time stamp>
plugs:
  <list of plugs>
slots:
  <list of slots>
```

Provide a reason for each plug and slot you want to use.

The app-privileges declaration together with its signature is simply appended to each .signature file.

#### Example

A full example of a .signature file:

```
type: app-revision
app-name: mockcorp-my-app
publisher: com.ctrlx-automation.app.mockcorp
organization: Mockcorp Limited
timestamp: 2024-11-18T08:15:04Z
snap-digest: 52a33b17a7e578a553d46a2bb4f3ce549f15ad06742580dcba2eea138f122eae3255d4842f8ea597a3cab72b9a301791

MEYCIQCNrOfqwizQtTm+UVwx2Q4Ee5uHiMMzQYnmOhNMoODYfwIhAJGloXM7lifS
vj1TVgv6DDaIAPuDCwhnKsrS3uuHSByN

type: app-privileges
app-name: mockcorp-my-app
publisher: com.ctrlx-automation.app.mockcorp
timestamp: 2024-11-18T08:15:04Z
plugs:
  firewall-control:
  loop-devices:
    interface: custom-device
    custom-device: loop-devices
slots:
  loop-devices-slot:
    interface: custom-device
    custom-device: loop-devices
    devices:
      - /dev/loop[0-9]
      - /dev/loop[1-9][0-9]
    files:
      read:
        - /proc/devices

MEYCIQCNrOfqwizQtTm+UVwx2Q4Ee5uHiMMzQYnmOhNMoODYfwIhAJGloXM7lifS
vj1TVgv6DDaIAPuDCwhnKsrS3uuHSByN
```

### Tools

Bosch Rexroth AG provides you with tools to sign your own apps after receiving your own OEM certificate from Bosch Rexroth AG. The following tools support you to integrate the signing process into your CI pipeline, feel free to adapt the scripts to your needs.

#### Signing tool

To sign apps, use the [`app-signing-tool.sh`](https://github.com/boschrexroth/ctrlx-automation-sdk/tree/main/scripts/app-signing-tool.sh) script. It accepts the following parameters:
- **Parameter 1:** OEM certificate (issued by Bosch Rexroth)
- **Parameter 2:** Signing Key (private key of the OEM)
- **Parameter 3:** Directory where the snaps to sign are located
- **Parameter 4:** Output directory for the signed apps (optional, defaults to Parameter 3)

The script packages all snap files, with the same name and version (different architectures), into the same .app file and signs them by creating a signature file for each architecture. The signature file is packaged into the .app and contains the signature, as well as some metadata derived from the provided OEM-Certificate.

The script expects the snaps to follow the naming scheme output by snapcraft when building a snap: `<app-name>_<app-version>_<architecture>.snap`

Example: **"myapp_1.4.3_arm64.snap"**

If you have a signed app-privileges declaration for your app, place this in the directory you defined in parameter 3. The privileges file needs to have the name `<app-name>.privileges`, e.g. `myapp.privileges`.

#### Verfiy tool

The [`verify-apps.sh`](https://github.com/boschrexroth/ctrlx-automation-sdk/tree/main/scripts/verify-apps.sh) script can then be used to verify the signatures of the resulting apps. It accepts the following parameters:

- **Parameter 1:** OEM Certificate
- **Parameter 2:** Directory where the apps to be validated are located
