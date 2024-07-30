# Document Overview

This document is part of the Bosch Rexroth ctrlX OS App Development Guideline, and describes which interfaces (port, plugs and slots) are reserved and may not be used by a ctrlX OS App

Please refer to the App Development Guide for further information.

The content of this document may be subject of change in future.

# 1. Reserved slots and plugs



The following operating system interfaces are denied or restricted for usage. See also https://snapcraft.io/docs/supported-interfaces.

| Interface | Direction | Parameters | Exceptions | Reason |
| --- | --- | --- |--- | --- |
| system-files | Plug | null | "'read=^(\\/dev\\/shm\| \\/dev\\/shm\\/.+)', 'write=^(\\/dev\\/shm\| \\/dev\\/shm\\/.+)$'" | Prevent access to system files, except shared memory |
|account-control|Plug|null|null|Prevent uncontrolled access to user and group databases|
|snapd-control|Plug|null|null|Prevent uncontrolled access to system configuration|
|content|Plug|content=package-certificates|null|Prevent apps to act as device admin|
|content|Plug|content=package-run|null|Prevent apps to act as device admin|
|content|Slot|content=active-solution|null|Prevent apps to act as solution snap|
|content|Slot|content=auth-service|null|Prevent unauthorized access, service to service authentication|
|content|Slot|content=rda-proxy-service|null|Prevent uncontrolled access to proxy service|
|content|plug|content=rda-proxy-service|null|Prevent unacontrolled access to proxy service|
|block-devices|plug|null|null|Prevent mounting of partitions|
|raw-volumes|Plug|null|null|Prevent mounting of partitions|
|udisk2|Slot|null|null|Prevent mounting of partitions|
|udisk2|Plug|null|null|Prevent mounting of partitions|
|docker|Plug|null|null|Prevent access to docker socket|
|docker-support|Slot|null|null|Prevent operating as the docker daemon because of possibility of privileg escalation|
|docker-support|Plug|null|null|Prevent operating as the docker daemon because of possibility of privileg escalation|
|acrn-support|Plug|null|null|Not being supported and potential dangerous|
|bool-file|Plug|null|null|Bool files are normally located in /sys - no write access to this folder|
|browser-support|Plug|null|null|Denylist|
|classic-support|Plug|null|null|Classic support bypass all snapd security mechanism|
|core-support|Plug|null|null|Deprecated|
|cpu-control|Plug|null|null|Allows setting CPU tunables|
|daemon-notify|Plug|null|null|Allows sending daemon status changes to the service manager.|
|dm-crypt|Plug|null|null|Storage support including encryption is part of the DeviceAdmin|
|firewall-control|Plug|null|null|Firewall app handles firewall rules|
|fpga|Plug|null|null|Allows access to fpga subsystem|
|use-support|Plug|null|null|Allows access to fuse subsyten|
|fwupd|Plug|null|null|Enabling privileged access to update UEFI firmware|
|gconf|Plug|null|null|Allows configuration of Gconf, typically used by old GNOME libs|
|gpg-keys|Plug|null|null|Allows reading GNU Privacy Guard (GPG) user configuration and keys as well as enabling GPG’s random seed to be updated|
|gpio-control|Plug|null|null|Super-privileged access to gpio pins|
|gpio-memory-control|Plug|null|null|Allows write access to all GPIO memory|
|greengrass-support|Plug|null|null|Allows operating as the Greengrass service to access resources and syscalls necessary to run Amazon Greengrass services and lambda functions|
|hardware-random-control|Plug|null|null|Enables control over the hardware random number generator by allowing read/write access to /dev/hwrng|
|hidraw|Plug|null|null|Enables raw access to USB and Bluetooth Human Interface (hidraw) devices. This interface is restricted because it provides privileged access to hardware devices|
|home|Plug|null|"read:all"|Allows access to non-hidden files owned by the user in the user’s home directory. There is no home folder for the users on ctrlX CORE|
|hugepages-control|Plug|null|null|Handle memory is part of the DeviceAdmin|
|intel-mei|Plug|null|null|Enables access to the Intel MEI management interface|
|io-ports-control|Plug|null|null|Allows access to all I/O ports, including the ability to write to /dev/port to change the I/O port permissions, the privilege level of the calling process, and disabling interrupts|
|ion-memory-control|Plug|null|null|Allows access to the Android ION memory allocator, a Linux kernel feature for managing one or more memory pools|
|kernel-module-control|Plug|null|null|Provides the ability to insert, remove and query kernel modules. This interface gives privileged access to the device|
|kernel-module-load|Plug|null|null|Provides the ability to load, or deny loading, specific kernel modules. This interface gives privileged access to the device|
|kubernetes-support|Plug|null|null|Allows operating as the Kubernetes service and running application containers|
|kvm|Plug|null|null|Allows access to the /dev/kvm device, providing privileged access and control of the KVM hypervisor|
|libvirt|Plug|null|null|Enables access to the libvirt control socket, which gives privileged access to control libvirtd on the host|
|login-session-control|Plug|null|null|Allows setup of login sessions and grants privileged access to user sessions.|
|lxd|Plug|null|null|Allows access to the LXD API via the socket provided by the lxd snap|
|lxd-support|Plug|null|null|Enables operating as the LXD service|
|microstack-support|Plug|null|null|Interface enables multiple service access for the Microstack infrastructure|
|mount-control|Plug|null|"Except storage-extension concept"|Allows the mounting and unmounting of both transient (non-persistent) and persistent filesystem mount points. This interface gives privileged access to the device|
|multipass-support|Plug|null|null|Allows operating as the Multipass service|
|netlink-audit|Plug|null|null|Allows access to the kernel part of the Linux Audit Subsystem through Netlink|
|netlink-connector|Plug|null|null|Allows communication through the kernel Netlink connector|
|netlink-driver|Plug|null|null|Allows a kernel module to expose itself to user-space via the Netlink protocol, typically to transfer information between the kernel and user-space processes|
|network-control|Plug|null|null|Gives access to wifi credentials|
|network-manager|Plug|null|null|Gives access to wifi credentials|
|network-manager-observe|Plug|null|null|Gives access to wifi credentials|
|network-setup-control|Plug|null|null|Gives access to wifi credentials|
|network-setup-observe|Plug|null|null|Gives access to wifi credentials|
 |packagekit-control|Plug|null|null|Allows control of the PackageKit service, giving privileged access to native package management on the system|
|password-manager-service|Plug|null|null|Provides access to the global password manager services provided by popular desktop environments, such as Secret Service and Kwallet|
|personal-files|Plug|null|null|Provides access to the specified files in the user’s home. This interface gives privileged access to the user’s data|
|physical-memory-control|Plug|null|null|Allows write access the to physical address space via the /dev/mem device|
|physical-memory-observe|Plug|null|null|Allows read access the to physical address space via the /dev/mem device|
|polkit-interface|Plug|null|null|Provides daemons with the permission to use the polkit authorisation manager (polkitd) to make access control decisions for requests from unprivileged clients|
|process-control|Plug|null|null|Prevent tampering with running processes|
|scsi-generic|Plug|null|null|Allows read and write access to SCSI Generic driver (sg) devices|
|sd-control|Plug|null|null|Allows for the management and control of SD cards on certain devices using the DualSD driver|
|shared-memory|Plug|null|"shared-memory=^(datalayer-shm)$"|Allows two snaps to communicate with each other using a specific predefined shared-memory path or directory in /dev/sh|
|snap-refresh-control|Plug|null, "Exceptions":null |Super privileged interface to allow extended control, via snapctl, of refreshes targeting the snap|
|ssh-keys|Plug|null|null|Allows a user’s SSH (Secure Socket Shell) configuration to be read, along with both their public and private keys|
|storage-framework-service|Plug|null|null|Allows operating as, or interacting with, the Storage Framework - storage is part of the DeviceAdmin|
|system-backup|Plug|null|null|Provides read-only access to the system via /var/lib/snapd/hostfs. This interface gives privileged access to system data|
|system-trace|Plug|null|null|Enables the monitoring and control of any running program, via kernel tracing facilities. This interface is restricted because it gives privileged access to all processes on the system and should only be used with trusted apps|
|tee|Plug|null|null|Permits access to Trusted Execution Environment (TEE) devices via the TEE subsystem in the Linux kernel|
|tpm|Plug|null|null|tpm allows access to the Trusted Platform Module (tpm) device, /dev/tpm0, and to the in-kernel resource manager, /dev/tpmrm0, on recent kernels (at least v4.12)|
|uhid|Plug|null|null|Enables the creation of kernel USB Human Interface Devices (HID) from user-space, via /dev/uhid , giving privileged access to HID transport drivers|
|uinput|Plug|null|null|Super privileged interface to allows write access to /dev/uinput on the host system for emulating input devices from userspace that can send input events|
 

 # 2. Blocked ports

The following ports are blocked and cannot be used by an app.

| Reserved by | Reserved ports |
| --- | --- |
| ctrlX OS | 22, 80, 81, 443, 1338, 1880, 1900, 2069, 2070, 4840, 5355, 5353, 5858, 6000, 7878, 8069, 11740, 11741 |
| Other apps | 1881, 1884, 1885, 2883, 4222, 6123, 7505, 9230, 9240, 8000, 8080, 8088, 8142, 8840, 8883, 10123, 16620, 16700, 16701, 16800, 16810, 18500, 47808, 48898, 49250-50250, 56090, 58000, 51218 |

 ****
**Copyright**
© Bosch Rexroth AG 2023 - 
This document, as well as the data, specifications and other information set forth in it, are the exclusive property of Bosch