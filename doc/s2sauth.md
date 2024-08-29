Service to Service (S2S) authentication provides an easy-to-use interface for applications to retrieve a JSON web token (JWT). By using this token, applications can access data from other resources.

In order to integrate service-to-service authentication into your application follow the instructions below.

!!! important
    Service-to-Service authentication is not available for apps which are installed from "unkown sources". Only apps which have been validated & digitally signed by Bosch Rexroth AG are permitted to use the interface.

## Add Content Plug to snapcraft.yaml

To be able to retrieve a token, the app has to provide two content slots. The definition for those slots must be added to the snapcraft.yaml:

```yaml
slots:
  secure-assets:
    interface: content
    content: secure-assets
    source:
      read:
        - $SNAP/secure-assets/${SNAPCRAFT_PROJECT_NAME}
  service-token:
    interface: content
    content: service-token
    source:
      write:
        - $SNAP_DATA/service-token/${SNAPCRAFT_PROJECT_NAME}
```

!!! important
    The target of the secure-assets slot must point to a location in the read-only area of the application ($SNAP, see example above). The app validation will fail if this rule is violated.

### Define & Provide Permissions (secure-assets)

The application must provide a file named `${SNAP_NAME}.scopes.json` located in `$SNAP/secure-assets/${SNAPCRAFT_PROJECT_NAME}` which defines the permissions that are requested by the application. One example would be:

```json
{
  "id": "myapp",
  "required-permissions": ["logbook.diag.r", "rexroth-deviceadmin.web.licensemanager.r"]
}
```

In this example, your app would request a token which provides access to the logbook (read-only) and the License Manager backend (read-only).

!!! important
    The parameter `id` must match the id that is defined in the Package Manifest (refer to [Package Manifest documentation](package-assets.md) for details)

### Retrieve and Use Authentication Token (service-token)

During runtime, the token will be provided to the application as a file named `<id>.token` (in the example shown above, this would be `myapp.token`) in the service-token folder (`$SNAP_DATA/service-token/${SNAPCRAFT_PROJECT_NAME}`).

!!! important
    The content of the token file might not be up-to-date when the application is ready, especially after a (re-)start of the system. Using an outdated token will lead to an authentication failure. Therefore, the application must implement a logic which - if the authentication request has failed - parses the token file again and tries to re-authenticate. This must be repeated until the authentication succeeds.
