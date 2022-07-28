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

MIT License

Copyright (c) 2022 Bosch Rexroth AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
