# Example automation.scheduler

## Introduction

The sample __automation.scheduler__  add a callable to the callable-pool of the control. The callable-pool can be found in the ctrlX Data Layer.

```
|-- framework
|-- scheduler
    |-- admin
    |-- callable-pool
        |-- <callable-name>
        |-- plc
        |-- motion
        |-- ...
    |-- groups
    |-- tasks
|-- ...
```
You can add each callable to a task, to run it.

## Pre-requisites

* Buildenvironment WSL or Virtual Box
* ctrlX AUTOMATION SDK Version 1.2 
* ctrlX CORE<sup>virtual</sup> or ctrlX CORE
* REST-Client 

## Using REST-Interface to run callable

Each REST-Call have to be done with the IP-adress of the ctlrX CORE e.g. `192.168.1.1` and an authorized user for example the default username `boschrexroth` and password `boschrexroth`.

### 1. Get access-token
   
```json
POST https://192.168.1.1/identity-manager/api/v1/auth/token
{
    "name": "boschrexroth",
    "password": "boschrexroth"
}
```
The response should be look like:

```json
{
        "access_token": "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE1OTM2MzQ2NDMsImlhdCI6MTU5MzYwNTg0MywiaWQiOiIxMDAxIiwibmFtZSI6ImJvc2NocmV4cm90aCIsIm5vbmNlIjoiOTI2OThhOTctYzk2My00M2IwLTk5NDMtODc5NjYzNjU2M2Q1IiwicGxjaGFuZGxlIjowLCJzY29wZSI6WyJyZXhyb3RoLWRldmljZS5hbGwucnd4Il19.fEtE_4yL2uOcZPcG6XkB6-0eHGZIuMbk1HHpx6B1dPeMxMVyzg6pqw28lv3yJfVdtnOY7G9P4IQjCHQyrMk8ImHLBWjW-3cf-XECajfMJISIgx5vmQ-tdk7rxsKOZjs0GjdD-vbCzoD93HwMwvksFb_-W6wLs8LsKErwPWbf_PotajXTDInG5Vyit4NlzLk1ZQWkn8V8FNWgFoXKd5gsHn44IrmR6MPqKXvg_jPxaw_-CDu7tCgvRpCGRR5_Sq3d0AC5sLPh5SJ1w9Mg-m2O7Ec2CCvWYF08VbRo_CEEDbcT2X6nkrzi7vyo6j1YWOc8RCBOB08z6GlHrg_IPipbRBzptRvTKD_4oHfEASInExEu7cxp2Oz0u9Nt7oZEFes3M47CDQJB9qhLweUbS9W_ZK5GQmDuVYA8eY3JUA_rOwcBeBzqZwb8mJpSJUYSofI8JXw37P8GUbj2YQwEAPnFkqbgSd_JIbAILVE7UtIQhmPETb1FpwId78cPbOzObfyJMM3gRdQo5XLSS5qPkz5KJqw2orJBkHC9Ysd9hcSSIl-eJoEBlsngvN4KfWqD5OvV2FHG1PVpEJX-TRLtb18jxnUHLM6Lr55wmLWy2cEFFtUnqPw2uGeF7Wlj6othiWliApZmF1sFjm6wt-Bdn07VIKWhka4klGoTZgP8OytWTc0",
        "token_type": "Bearer"
}
```

Add the token to your authorization header with type `Bearer` to each of the following https-requests.

### 2. Create a new task (scheduler state must be in config)
   
```json
POST https://192.168.1.1/automation/api/v1/scheduler/tasks/
{
    "cycletime": 100000,
    "event": "cyclic",
    "name": "HelloCallable"
}
``` 

Only the name is required.
The possible task configurations are: 

```
Task {
        name: string (required); // singleton
        priority: uint32 = 119; // 0=highest .. 139=lowest
        affinity: uint32 = 0; // cpu affinity 0..3 (max)
        stacksize: uint32 = 131072; // bytes
        event: string (required); // e.g. "cyclic"
        cycletime: uint32 = 20000; // microseconds
     }
```

### 3. Add the callable to a task
   
```json
POST https://192.168.1.1/automation/api/v1/scheduler/tasks/HelloCallable/program/
{
    "arguments": [
    ],
    "index": 0,
    "name": "HelloCallableFactory"
}
```

The argumentlist is not necceassary and can be empty.

```
Callable {
                name: string (required); // singleton (callable factory name)
                index: uint32 = 0; // call index 1 (highest) to maximum type range (default: 0 = lowest)
                arguments: [string]; // values to initialize the callable
         }
```

The response contains the information about the program index:

Response: 

```json
{
  "type": "string",
  "value": "scheduler/tasks/HelloCallable/program/2/"
}
```

### Additional task informations are located in the ctrlX Data Layer

```
|-- scheduler
    |-- admin
    |-- callables-pool
    |-- groups
    |-- tasks
        |-- HelloCallable
            |-- info
            |-- program     // Contains the programindex, needed to delete the callable from the task
            |-- properties
            |-- state
            |-- watchdog
|-- ...
```

To delete a callable from a task, use:

```json
DELETE https://192.168.1.1/automation/api/v1/scheduler/tasks/HelloCallable/program/2
```



To delete a task use:

```json
DELETE https://192.168.1.1/automation/api/v1/scheduler/tasks/HelloCallable
```

## Screenshots

![Callable-Factory](docs/images/automation.scheduler/callable.png)
![Diagnosis](docs/images/automation.scheduler/diagnosis_tick.png)


## Support

If you've any questions visit the [ctrlX AUTOMATION Community](https://developer.community.boschrexroth.com/)
___


## License

MIT License

Copyright (c) 2020-2021 Bosch Rexroth AG

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