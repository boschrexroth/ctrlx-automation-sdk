# README monitor-health

This sample shows how to provide easy health monitoring of an production app using an observation script
started as an second app service in parallel to monitor the health of your main app.

This is monitoring the state of the service and setting the health.

| Status    | Health    | Message       | Code
|:----------|:----------|:--------------|:--------------|
|active     |okay       |  -            |-
|inactive   |error      |app stopped!   |-
|disabled   |error      |app disabled!  |-

## Description

The sample contains a dummy production app _./app/dummy-app_, which is monitored for it's health.

## Setup health monitoring for your own app

+ Copy the script  _./health/check-health_ to your project to folder ./health.
+ Change the name of your snap (here: 'ctrlx-monitor-health') and your production app (part-name, here: 'app') to be observed. Ensure Linux line endings (LF).
+ Make script  _./health/check-health_ executable:

        sudo chmod +x ./health/check-health

+ Edit _./snap/snapcraft.yaml_ and add the script as a health observation service to app.

        apps:
        check-health:
            command: check-health
            daemon: simple
            restart-condition: on-failure
            passthrough:  
            restart-delay: 10s
        ...
        
        parts:
        check-health:
            plugin: dump
            source: ./health

## Support

### Developer Community

Please join the [Developer Community](https://developer.community.boschrexroth.com/)

### SDK Forum

Please visit the [SDK Forum](https://developer.community.boschrexroth.com/t5/ctrlX-AUTOMATION/ct-p/dcdev_community-bunit-dcae/)

### Issues

If you've found an error in these sample, please [file an issue](https://github.com/boschrexroth)

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT
