This document is part of the Bosch Rexroth ctrlX OS App Development Guideline, and describes how a ctrlX OS App can integrate in the ctrlX OS System Report

Please refer to the App Development Guide for further information.

The content of this document may be subject of change in future.


# Introduction
The ctrlX OS system report is provided as zip file. ctrlX OS Apps can participate in this mechanism and provide their own log information and other files.
This document describes, which interfaces are available in ctrlX OS to automatically add App specific information to the system report.

**Note**: This mechanism is available in ctrlX OS 1.20 or higher

# Integration in the ctrlX OS System Report

## Information in snapcraft.yaml

- "**apps**" section: add "package-run" to the slots of your service(s) that will add files to the system report
- "**slots**" section: add the "package-run" sections as described below:

```yaml

apps:
  ...
  my-service:
    slots: [..., package-run, ...]

...

slots:
  ...
  package-run:
    interface: content
    content: package-run
    source:
      write:
        - $SNAP_DATA/package-run/${SNAPCRAFT_PROJECT_NAME}

```

## Files for System Report

The files, which will be part of the System Report, will be provided in this directory:

```yaml
$SNAP_DATA/package-run/$SNAP_INSTANCE_NAME/logs
```
- Access mode for files: 0644
- Access mode for the directory: 0755


# Example
With    
```yaml
$SNAP_INSTANCE_NAME: my-snap
$SNAP_DATA: /var/snap/my-snap/current
```
and the directory structure

```yaml
drwxr-xr-x  root root  /var/snap/my-snap/current
drwxr-xr-x  root root  /var/snap/my-snap/current/package-run
drwxr-xr-x  root root  /var/snap/my-snap/current/package-run/my-snap
drwxr-xr-x  root root  /var/snap/my-snap/current/package-run/my-snap/logs
-rw-r--r--  root root  /var/snap/my-snap/current/package-run/my-snap/logs/my-log-file.log
drwxr-xr-x  root root  /var/snap/my-snap/current/package-run/my-snap/logs/my-dir
-rw-r--r--  root root  /var/snap/my-snap/current/package-run/my-snap/logs/my-dir/my-other-file.dat
drwxr-xr-x  root root  /var/snap/my-snap/current/package-run/my-snap/logs/my-empty-dir
```
the following files and directories are added to the system report:

```yaml
my-log-file.log
my-dir/my-other-file.dat
my-empty-dir
```
