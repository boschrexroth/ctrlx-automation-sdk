# README Node.js datalayer.client.browse

This Node.js sample writes out all node values from ctrlX Data Layer to console.

## Introduction

The sample demonstrates how to traverse the whole ctrlX Data Layer tree. The values of the nodes if readable are printed out to console as human readable strings. Also complex values with structure (encoded by [FlatBuffers](https://google.github.io/flatbuffers/)) are written as JSON string to console.

## Description

The sample __datalayer.client.browse__ demonstrates

+ How to browse nodes of the __ctrlX Data Layer__
+ How to read value as json from a node

## Debug and Run

1. Run 'npm install' in the terminal.  
2. Launch the app with the debugger attached by pressing __F5__.

## Build and Install

1. Launch Visual Studio Code
2. Click on the __Remote Explorer__ from the menu.
3. Choose __Open Folder__.
4. Open the directory __datalayer.client.browse__.
5. Build and install snap as described [here](./../README.md).
6. Check the output using __Diagnostics__:

   + Login into your ctrlX
   + Navigate to __Diagnostics -> Logbook__
   + Click __Settings__ on the right top corner
   + Enable __Show system messages__
   + Navigate to __Filter -> Units__ and check your __snap.sdk-node-client-browse.app.service__
   + Now you should the see the app output diagnostics.
   + Press __Refresh__ to update.

7. If you have root permissions, you can also watch the output on any ssh console with the command:

   ```bash
   sudo snap logs sdk-node-client-browse.app -f | more
   ```

## Console Output

```bash
Running inside snap: False
ctrlX Data Layer system started.
ctrlX Data Layer client created.
Client connected: True
\-[]
  |-[datalayer]
  | |-[curvemq]
  | | \-[publickey] V=tGQ43.RsrvZ#K0]eLTs?vpyfCNN?^/sV6MGh7N
  | |-[debug]
  | |-[nodes] [datalayer/debug/*,datalayer/curvemq/publickey,datalayer/nodes,datalayer/nodesrt,datalayer/subscriptions/clients/*,datalayer/subscriptions/clients/*/subscriptions/*,datalayer/subscriptions/clients/*/subscriptions/*/nodes,datalayer/subscriptions/clients/*/subscriptions/*/properties,datalayer/subscriptions/settings,datalayer/retain/emu/data,datalayer/retain/emu/data/__,datalayer/retain/emu/info,datalayer/retain/emu/map,datalayer/retain/chunks,datalayer/retain/chunks/__,datalayer/retain/stats,datalayer/retain/stats/free,datalayer/retain/stats/used,datalayer/retain/stats/total,datalayer/retain/stats/biggest-free,datalayer/retain/stats/info,datalayer/retain/stats/sync-counter,datalayer/retain/stats/last-used,types/datalayer/metadata,types/datalayer/reflection,types/datalayer/memory,types/datalayer/memory_map,types/datalayer/token,types/datalayer/problem,types/datalayer/retain_stats,types/datalayer/subscription_properties,types/datalayer/subscription_data,types/datalayer/subscription_settings,types/datalayer/persistence_param,types/datalayer/subscription,types/datalayer/bool8,types/datalayer/int8,types/datalayer/uint8,types/datalayer/int16,types/datalayer/uint16,types/datalayer/int32,types/datalayer/uint32,types/datalayer/int64,types/datalayer/uint64,types/datalayer/float32,types/datalayer/float64,types/datalayer/string,types/datalayer/array-of-bool8,types/datalayer/array-of-int8,types/datalayer/array-of-uint8,types/datalayer/array-of-int16,types/datalayer/array-of-uint16,types/datalayer/array-of-int32,types/datalayer/array-of-uint32,types/datalayer/array-of-int64,types/datalayer/array-of-uint64,types/datalayer/array-of-float32,types/datalayer/array-of-float64,types/datalayer/array-of-string,types/datalayer/raw,types/datalayer/remoteconfig,types/datalayer/framework/bundle,types/datalayer/framework/bundlelist,types/datalayer/framework/dependency,types/datalayer/framework/dependencylist,types/datalayer/framework/interface,types/datalayer/framework/interfacelist,types/datalayer/framework/component,types/datalayer/framework/componentlist,types/datalayer/framework/property,types/datalayer/framework/propertylist,types/diagnosis/cfg-class,types/diagnosis/identification,types/diagnosis/list-identification-timestamp,types/diagnosis/identification-timestamp,types/diagnosis/registration,types/diagnosis/set,types/scheduler/callable,types/scheduler/duration,types/scheduler/duration-timer,types/scheduler/histogram,types/scheduler/programs,types/scheduler/state,types/scheduler/task,types/scheduler/trigger-source,types/scheduler/startup-error-reaction,types/scheduler/admin,types/scheduler/watchdog-type,types/scheduler/watchdog-error-reaction-class,types/scheduler/watchdog-error-reaction-configuration,types/scheduler/watchdog,types/scheduler/controls,types/systemhandler/state,types/script/cfg,types/script/cfg/init-script,types/script/instances,types/script/instances/cmd/file,types/script/instances/cmd/string,types/script/instances/diag,types/script/state/languages,devices/remotes/__,framework/bundles,framework/bundles/__,framework/metrics/system/cpu-utilisation-percent,framework/metrics/system/memtotal-mb,framework/metrics/system/memfree-mb,framework/metrics/system/memavailable-mb,framework/metrics/system/memused-mb,framework/metrics/system/membuffers-mb,framework/metrics/system/memcache-mb,framework/metrics/system/memused-percent,framework/metrics/process/vmsize-kb,framework/metrics/process/vmrss-kb,framework/metrics/process/vmshared-kb,framework/metrics/process/memused-percent,framework/state,framework/events/framework-started,diagnosis/cfg/realtime/numbers,diagnosis/cfg/realtime/numbers/class,diagnosis/clear/error,diagnosis/confirm/error,diagnosis/get/actual/list,diagnosis/get/actual/log,diagnosis/get/text,diagnosis/registration/detailed/register,diagnosis/registration/detailed/unregister,diagnosis/registration/main/register,diagnosis/registration/main/unregister,diagnosis/set,diagnosis/set/reset-active,diagnosis/set/set-active,diagnosis/set/set-and-reset,trace/rexroth-automationcore/units,trace/rexroth-automationcore/units/__,trace/rexroth-automationcore/admin/cfg/*,scheduler/tasks/*/watchdog,scheduler/tasks/*/watchdog/__,scheduler/__,script/cfg,script/cfg/load-all,script/cfg/save-all,script/cfg/init-script,script/cfg/init-script/file,script/cfg/init-script/language,script/cfg/init-script/parameter,script/admin/cfg/load,script/admin/cfg/save,script/instances/*,script/instances/*/cmd/file,script/instances/*/cmd/string,script/instances/*/cmd/reset,script/instances/*/cmd/abort,script/instances/*/state/opstate,script/instances/*/state/script,script/instances/*/state/line,script/instances/*/state/attached-to,script/instances/*/state/pid,script/instances/*/diag,script/instances/*/diag/last-main-diag,script/instances/*/diag/last-detail-diag,script/instances/*/diag/err-msg,script/instances/*/diag/err-trace,script/state/languages/*]
  | |-[nodesrt] [datalayer/retain/emu]
  | |-[retain]
  | | |-[chunks] []
  | | |-[emu]
  | | | |-[data] []
  | | | |-[info] {"type": "Shared","id": "/snap.rexroth-automationcore.405e2e7a-63ae-0039-056b-deadbe000002","sizeBytes": 122880}
  | | | \-[map] {"revision": 0}
  | | \-[stats] {"total": 122880,"free": 122848,"used": 32,"biggestFree": 122848,"syncCounter": 124,"lastUsed": 32}
  | |   |-[biggest-free] 122848
  | |   |-[free] 122848
  | |   |-[info] emulated
  | |   |-[last-used] 32
  | |   |-[sync-counter] 124
  | |   |-[total] 122880
  | |   \-[used] 32
  | \-[subscriptions]
  ...
  Elapsed time for browsing ctrlX Data Layer tree: 9351 milliseconds
```

## Install the App

Login into ctrlX and install the App (Apps).

## Gratulations - We're finished - Let's start coding

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
