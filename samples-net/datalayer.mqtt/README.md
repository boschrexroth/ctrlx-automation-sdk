# README .NET datalayer.mqtt

This .NET sample connects the __ctrlX Data Layer__ to a __MQTT 5__ broker.
The sample is based on [MQTT.net](https://github.com/chkr1011/MQTTnet).

__IMPORTANT__:

The sample creates and registers all nodes individually, which is recommended for _small_ to _medium_ sized set of nodes to handle. If you're dealing with a _large_ set of nodes (big data provider) or any _dynamic node hierarchy_, we recommend to use a _virtual provider_ approach, listening just to any _wildcarded address_ ( e.g. myRoot/**) and using _self-managed virtual nodes_, just returned in _OnBrowse()_ method, which is more efficient.

Please read the _Best Practise_ section in common .NET documentation for more details and recommended samples.

## Introduction

MQTT sample showing PUB/SUB with ctrlX Data Layer in full interaction.

Connect Datalayer to any MQTT Broker for data synchronization (SUB) or publishing (PUB) or even full broker synchronisation (__Topic Scanner__).

A simple use-case could be any kind of M2M or M2Cloud communication as an light-weight alternative to full stack OPC UA Pub/Sub.


## Prerequisites

Please use this sample in conjunction with any MQTT Broker supporting __MQTT 5__.
We recommend to use [mosquitto MQTT Broker](https://mosquitto.org/download/), which can be also installed on a _ctrlX_ as snap

    snap install mosquitto

## Description

The MQTT sample demonstrates 

+ How easy it is to interact __ctrlX Data Layer with MQTT__
  
+ A proposal to organize a set of logical grouped provided nodes using a handler hierarchy and how to populate nodes using some __level node handlers__ , responsible for it's own set of children nodes 
  
+ How to create an __light-weight node handler__, returning just his own managed _virtual_ nodes on _OnBrowse()_ without the need of registering all nodes one-by-one which is very efficient for a large set of dynamically created nodes.
  
+ A concept of creating configurable __PUB configurations__, publishing the value of a ctrlX Data Layer source address to a MQTT topic.
  
+ A concept of creating configurable __SUB configurations__, subscribing a MQTT topic (wildcards supported) and writing it back to a ctrlX Data Layer target address. 

+ On top a fully functional __Topic Scanner__, scanning all topics present on the broker and revealing it into the address space of the ctrlX Data Layer, which is very handy for other ctrlX Data Layer clients to access MQTT related data _immediately_ without connecting to MQTT itself. The most recent message of the topic will be memory-persisted as and accessible via a simple ctrlX Data Layer address.

__Notes__: 

+ The sample uses _unsecure_ MQTT communication. Please use a MQTT TLS setup in any production environment. See broker documentation for details.
+ The sample _does not provide any persistence_ of created PUB or SUB configurations.

## Prerequisites

Please read the common .NET docs [here](./../../doc/dotnet.md), first.

## Getting Started

1. Launch Visual Studio Code
2. Click on the __Remote Explorer__ from the menu.
3. Choose __Open Folder__.
4. Open the directory __datalayer.mqtt__.
5. Build and install snap as described [here](./../README.md).
6. Checkout ctrlX Data Layer web frontend (Settings | Data Layer) 
7. Check the output using __Diagnostics__:

   - Login into your ctrlX
   - Navigate to __Diagnostics -> Logbook__
   - Click __Settings__ on the right top corner
   - Enable __Show system messages__
   - Navigate to __Filter -> Units__ and check your __snap.ctrlx-dotnet-datalayer-mqtt.app.service__
   - Now you should the see the app output diagnostics.
   - Press __Refresh__ to update.

8. If you have root permissions, you can also watch the output on any ssh console with the command:
   ```bash
   sudo snap logs ctrlx-dotnet-datalayer-mqtt.app -f | more

## Install the App

Login into ctrlX and install the App (Apps).

## Gratulations - We're finished - Let's start coding!


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