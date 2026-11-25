## Introduction

To ensure legal compliance and transparency in the use of Free and Open Source Software (FOSS), every app that includes FOSS components must provide comprehensive license information. ctrlX OS collects this information and displays the summarized license details for all integrated apps inside the ctrlX web interface.

## Supported formats

FOSS license information can be provided in one of the following formats:

- **fossinfo**
- **CycloneDX BOM**

### fossinfo format

For more information about "fossinfo.json" please refer to the guideline in the SDK <span style="color:red;">**TODO: Where?**</span>.

If FOSS license texts are displayed within the app, at least a reference to the license display in the app must be provided in the "fossinfo.json".

### CycloneDX format

ctrlX OS supports CycloneDX BOM files of type JSON (**"cyclonedx.json"**). FOSS license information is collected from CycloneDX components. Below is a simple example of a "cyclonedx.json" file (the `@acme/component` below `metadata` is the component described by the file).

``` json
{
  "bomFormat": "CycloneDX",
  "specVersion": "1.2",
  "version": 1,
  "metadata": {
    "component": {
      "type": "library",
      "name": "@acme/component",
      "components": [
        {
          "type": "library",
          "name": "@acme/component1",
          "version": "1.0.0",
          "copyright": "Copyright (c) 2001 ACME Inc",
          "manufacturer": {
            "url": [
              "https://example.com/manufacturer"
            ]
          },
          "licenses": [
            {
              "license": {
                "name": "MIT License",
                "text": {
                  "content": "The MIT License\n\nCopyright (c) 2001 ACME Inc\n\nPermission is hereby granted, ..."
                }
              }
            }
          ]
        }
      ]
    }
  },
  "components": [
    {
      "type": "library",
      "name": "@acme/component2",
      "version": "2.0.0",
      "copyright": "Copyright (c) 2002 ACME Inc",
      "supplier": {
        "url": [
          "https://example.com/supplier"
        ]
      },
      "licenses": [
        {
          "license": {
            "name": "MIT License",
            "text": {
              "content": "The MIT License\n\nCopyright (c) 2002 ACME Inc\n\nPermission is hereby granted, ..."
            }
          }
        }
      ]
    }
  ]
}
```

To get the necessary FOSS license information, ctrlX OS supports the following CycloneDX properties:

- components: `name`, `version`, `copyright`, `supplier.url`, `manufacturer.url` (since v1.6)
- licenses: `name`, `text.content`

!!! important
    ctrlX OS does not provide the license text dependent on the license name or ID. Therefore, the license text must be provided by the CycloneDX BOM file via the license property `text.content`.

The component property `manufacturer` is supported since CycloneDX BOM v1.6. If `manufacturer` and `supplier` are both provided, the URL from the manufacturer is used and the supplier URL is ignored.

## Provisioning

The **"fossinfo.json"** file must be packaged inside the snap and stored at snap/package-assets/{{appName}}.fossinfo.json. The file must contain the OSS license information and corresponding license texts for all included open source software components.

For more details, refer to the [Open Source Software (OSS)](package-assets.md#open-source-software-oss) section in the package-assets documentation. In addition, the license texts must be listed within the user documentation or at least a reference must be inserted where the FOSS license texts are located.