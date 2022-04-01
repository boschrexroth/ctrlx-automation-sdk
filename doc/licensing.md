### Licensing

ctrlX World Partners must use the licensing manager service that is operated by the Bosch Rexroth AG. Licenses are issued by the Bosch Rexroth Licensing Center [https://licensing.boschrexroth.com](https://licensing.boschrexroth.com). There a user can assign licenses to devices and can download a capability response, containing all assigned licenses for a specific device. The downloaded capability response can than be deployed on the corresponding device.

On every ctrlX CORE we offer a UI to upload the provided capability response and to check the currently available licenses on the device. The license manager feature on the ctrlX CORE provides a restful API via an internal unix domain socket, that can be used by apps to request and release a specific license.

In order to integrate license enforcement into your application follow the instructions below:

### Add content plug to snapcraft.yaml
To get access to the unix domain socket that provides the restful API add the following content plug to the snapcraft.yaml:

```yaml
plugs:
  licensing-service:
    interface: content
    content: licensing-service
    target: $SNAP_DATA/licensing-service
``` 

This will create a folder "license-service" during installation of the app on the ctrlX CORE and provide access to the unix domain socket "licensing-service.sock".

### Use the restful API to enforce licenses:
The description of the API is avaiable on GitHub: [https://boschrexroth.github.io/rest-api-description](https://boschrexroth.github.io/rest-api-description)

Every device has its own license pool. If an app wants to enforce a license, it has to aquire one license out of the pool. This can be done by POST command to the path /license-manager/api/v1/license.


!!! important   
    If the license is not needed anymore it has to be returned to the license pool! This is also necessary if an app is stopped or uninstalled. To do this, just use the DELETE    command to the path /license-manager/api/v1/license/{id} with the ID you got when the license was aquired. If an application crashes and the license was not returned properly a reboot of   the device is nessary. A reboot resets the license pool and all licenses can be acquired again. 

It is recommend to return the license back to the pool to aquire it again from time to time. With this method the application can recognise if an license was removed or expired in the meantime without dealing with systemtime.



## Licensing modes 
To ensure the useablity the licensing mechanism on the device has two special modes to reduce potential down time of an appliance. 

### Temporary usage rights
The user of the ctrlX CORE has the possibility to activate a special mode that is called "Temporary usage right" in the license manager. This mode is active for 10 days and allows the user to use any software on the device. This mode can only activated once and is meant for a disaster recovery on a new hardware. This method reduces the down time. The user has to add licenses to the device or move the licenses from the broken to new device within this 10 days.

The applications are informed due the set "tampered" flag in the response when acquiring such a license from the license manager. 

### Licensing on the ctrlX CORE<sup>virtual</sup>
Currently the user is not able to buy licenses for a ctrlX CORE<sup>virtual</sup> for productive use. To have the possiblity to run an app that enforces licenses on the ctrlX CORE<sup>virtual</sup> we have limited the runtime on the ctrlX CORE<sup>virtual</sup> to four hours and introduced an license that indicates that the device is currently in the limited evualtion mode with that limited runtime. After this 4 hours the device shuts down itself to prevent a productive  use.

In order to support this non-productive mode, apps must check for the license `SWL_XCR_ENGINEERING_4H`. If that license is available it is guaranteed that the runtime is limited to 4 hours.