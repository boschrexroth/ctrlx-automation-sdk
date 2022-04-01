#!/usr/bin/env node
// !!! DO NOT REMOVE THE SHEBANG ON TOP OF THE FILE, WHICH SPECIFIES THIS APP TO BE EXECUTED BY NODE.JS !!!

// Copyright (c) 2021-2022 Bosch Rexroth AG

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the 'Software'), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// DatalayerSystem
import DatalayerSystem from 'ctrlx-datalayer/dist/datalayersystem';
import Result from 'ctrlx-datalayer/dist/result';
import ProviderNodeCallbacks from 'ctrlx-datalayer/dist/providernodecallbacks';
import IVariant from 'ctrlx-datalayer/dist/variant';
import IProviderNodeResult from 'ctrlx-datalayer/dist/providernoderesult';

// Flatbuffers
import * as flatbuffers from 'flatbuffers';
import { InertialValue } from './sample/schema/inertial-value';

import DataRanges from './dataranges';
import DataTypes from './datatypes';
import Node from './node';


// The main function
async function main() {

    // This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:PORT
    // Please check and change according your environment:
    // - USER:       Enter your user name here - default is boschrexroth
    // - PASSWORD:   Enter your password here - default is boschrexroth
    // - IP_ADDRESS: 127.0.0.1   If you develop on your (Windows) host and you want to connect to a ctrlX CORE virtual with port forwarding
    //               10.0.2.2    If you develop on a VM (QEMU, Virtual Box) and you want to connect to a ctrlX virtual with port forwarding
    //               192.168.1.1 If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater
    const connectionTcp = 'tcp://boschrexroth:boschrexroth@10.0.2.2';

    // Please use the connection string for Inter Process Communication
    // for performance reasons when running inside a snap:
    const connectionIpc = DatalayerSystem.protocolSchemeIpc;

    // Check if the process is running inside a snap 
    const isSnapped = isSnap();
    console.log('running inside snap:', isSnapped)

    // Create a new ctrlX Data Layer system
    const system = new DatalayerSystem('');

    // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
    await system.start(false);

    // Set the remote address to inter-process communication (ipc) protocol if running in snap, otherwise tcp
    const remote = isSnapped ? connectionIpc : connectionTcp;
    console.log('provider remote address:', remote)

    //Create a Provider with the given remote address
    const provider = await system.createProvider(remote);
    await provider.start();

    if (provider.isConnected() === false) {
        console.log('provider is not connected -> exit.')
        return;
    }

    // Register type with binary flatbuffers schema file: sampleSchema.bfbs (auto generated from sampleSchema.fbs by flatc compiler)
    const typeAddressInertialValue = 'types/samples/nodejs/schema/inertial-value';
    const sampleSchemaFile = __dirname + '/sampleSchema.bfbs';
    await provider.registerType(typeAddressInertialValue, sampleSchemaFile).then(result => {
        console.log('Registering Type with address=', typeAddressInertialValue, ', result=', result);
    });

    const builder = new flatbuffers.Builder(DatalayerSystem.defaultFlatbuffersInitialSize);
    const offset = InertialValue.createInertialValue(builder, 30, -442, 911);
    builder.finish(offset);
    var inertialValue = builder.asUint8Array();

    // Root node for all static nodes.
    const staticRoot = 'samples/nodejs/alldata/static';

    // Create all static nodes.
    const staticNodes = [
        Node.createStatic(staticRoot, DataTypes.bool8, DataRanges.bool8.getMax()),
        Node.createStatic(staticRoot, DataTypes.int8, DataRanges.int8.getMax()),
        Node.createStatic(staticRoot, DataTypes.uint8, DataRanges.uint8.getMax()),
        Node.createStatic(staticRoot, DataTypes.int16, DataRanges.int16.getMax()),
        Node.createStatic(staticRoot, DataTypes.uint16, DataRanges.uint16.getMax()),
        Node.createStatic(staticRoot, DataTypes.int32, DataRanges.int32.getMax()),
        Node.createStatic(staticRoot, DataTypes.uint32, DataRanges.uint32.getMax()),
        Node.createStatic(staticRoot, DataTypes.int64, DataRanges.int64.getMax()),
        Node.createStatic(staticRoot, DataTypes.uint64, DataRanges.uint64.getMax()),
        Node.createStatic(staticRoot, DataTypes.float, DataRanges.float.getMax()),
        Node.createStatic(staticRoot, DataTypes.double, DataRanges.double.getMax()),
        Node.createStatic(staticRoot, DataTypes.string, 'String_0'),
        Node.createStatic(staticRoot, DataTypes.inertialValue, inertialValue),
        Node.createStatic(staticRoot, DataTypes.arbool8, [true, false, true]),
        Node.createStatic(staticRoot, DataTypes.arint8, [DataRanges.int8.getMin(), -1, 0, DataRanges.int8.getMax()]),
        Node.createStatic(staticRoot, DataTypes.aruint8, [DataRanges.uint8.getMin(), DataRanges.uint8.getMax()]),
        Node.createStatic(staticRoot, DataTypes.arint16, [DataRanges.int16.getMin(), -1, 0, DataRanges.int16.getMax()]),
        Node.createStatic(staticRoot, DataTypes.aruint16, [DataRanges.uint16.getMin(), DataRanges.uint16.getMax()]),
        Node.createStatic(staticRoot, DataTypes.arint32, [DataRanges.int32.getMin(), -1, 0, DataRanges.int32.getMax()]),
        Node.createStatic(staticRoot, DataTypes.aruint32, [DataRanges.uint32.getMin(), DataRanges.uint32.getMax()]),
        Node.createStatic(staticRoot, DataTypes.arint64, [DataRanges.int64.getMin(), -1n, 0n, DataRanges.int64.getMax()]),
        Node.createStatic(staticRoot, DataTypes.aruint64, [DataRanges.uint64.getMin(), DataRanges.uint64.getMax()]),
        Node.createStatic(staticRoot, DataTypes.arfloat, [DataRanges.float.getMin(), -1.0, 0.0, DataRanges.float.getMax()]),
        Node.createStatic(staticRoot, DataTypes.ardouble, [DataRanges.double.getMin(), -1.0, 0.0, DataRanges.double.getMax()]),
        Node.createStatic(staticRoot, DataTypes.arstring, ['Blue', 'Red', 'Orange'])
    ]

    // Root node for all dynamic nodes.
    const dynamicRoot = 'samples/nodejs/alldata/dynamic'

    // Create all dynamic nodes.
    const dynamicNodes = [
        Node.createDynamic(dynamicRoot, DataTypes.bool8, 0),
        Node.createDynamic(dynamicRoot, DataTypes.int8, 0),
        Node.createDynamic(dynamicRoot, DataTypes.uint8, 0),
        Node.createDynamic(dynamicRoot, DataTypes.int16, 0),
        Node.createDynamic(dynamicRoot, DataTypes.uint16, 0),
        Node.createDynamic(dynamicRoot, DataTypes.int32, 0),
        Node.createDynamic(dynamicRoot, DataTypes.uint32, 0),
        Node.createDynamic(dynamicRoot, DataTypes.int64, 0n),
        Node.createDynamic(dynamicRoot, DataTypes.uint64, 0n),
        Node.createDynamic(dynamicRoot, DataTypes.float, 0),
        Node.createDynamic(dynamicRoot, DataTypes.double, 0),
        Node.createDynamic(dynamicRoot, DataTypes.string, 'String_0'),
        Node.createDynamic(dynamicRoot, DataTypes.inertialValue, inertialValue),
        Node.createDynamic(dynamicRoot, DataTypes.arbool8, [true, false, true, false, true, false, true, false, true, false]),
        Node.createDynamic(dynamicRoot, DataTypes.arint8, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(dynamicRoot, DataTypes.aruint8, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(dynamicRoot, DataTypes.arint16, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(dynamicRoot, DataTypes.aruint16, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(dynamicRoot, DataTypes.arint32, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(dynamicRoot, DataTypes.aruint32, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(dynamicRoot, DataTypes.arint64, [-5n, -4n, -3n, -2n, -1n, 0n, 1n, 2n, 3n, 4n]),
        Node.createDynamic(dynamicRoot, DataTypes.aruint64, [0n, 1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n]),
        Node.createDynamic(dynamicRoot, DataTypes.arfloat, [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]),
        Node.createDynamic(dynamicRoot, DataTypes.ardouble, [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]),
        Node.createDynamic(dynamicRoot, DataTypes.arstring, ['String_0', 'String_1', 'String_2', 'String_3', 'String_4', 'String_5', 'String_6', 'String_7', 'String_8', 'String_9'])
    ]

    // Create map with key address and value node for the callback functions.
    var addressToNode = new Map<string, Node>()

    // Add all static nodes to the map.
    staticNodes.forEach(node => {
        addressToNode.set(node.getAddress(), node)
    });

    // Add all dynamic nodes to the map.
    dynamicNodes.forEach(node => {
        addressToNode.set(node.getAddress(), node)
    });

    // Implement the onMetadata callback function
    const onMetadata = (address: string, result: IProviderNodeResult) => {
        const node = addressToNode.get(address);
        if (node) {
            result.setResult(Result.OK, node.getMetadata(), 'flatbuffers');
        }
        else {
            result.setResult(Result.UNSUPPORTED, null);
        }
    };

    // Implement the onRead callback function
    const onRead = (address: string, args: IVariant, result: IProviderNodeResult) => {
        const node = addressToNode.get(address);
        if (node) {
            result.setResult(Result.OK, node.getValue(), node.getDataType().getVariantType());
        }
        else {
            result.setResult(Result.UNSUPPORTED, null);
        }
    };

    // Implement the onWrite callback function
    const onWrite = (address: string, args: IVariant, result: IProviderNodeResult) => {
        const node = addressToNode.get(address);
        if (node) {
            node.setValue(args.value);
            result.setResult(Result.OK, node.getValue(), node.getDataType().getVariantType());
        }
        else {
            result.setResult(Result.UNSUPPORTED, null);
        }
    };

    // Create provider node callbacks.
    const providerNodeCallbacks = new ProviderNodeCallbacks()
        .setOnMetadata(onMetadata)
        .setOnRead(onRead)
        .setOnWrite(onWrite);

    // Register static nodes
    staticNodes.forEach(async node => {
        await provider.registerNode(node.getAddress(), providerNodeCallbacks).then(
            (result) => console.log('registered static node', node.getAddress(), 'with result:', result))
    });

    // Register dynamic nodes
    dynamicNodes.forEach(async node => {
        await provider.registerNode(node.getAddress(), providerNodeCallbacks).then(
            (result) => console.log('registered dynamic node', node.getAddress(), 'with result:', result))
    });

    // Increment values of dynamic node every second.
    const updateIntervalHandle = setInterval(() => {
        if (provider.isConnected()) {
            // Increment the values of all dynamic nodes.
            dynamicNodes.forEach(async node => {
                node.incrementValue();
            });
        }
    }, 1000);

    //Keep the process alive until disconnected
    const intervalHandle = setInterval(() => {
        if (system.isStarted() === false || provider.isConnected() === false) {
            clearInterval(updateIntervalHandle);
            clearInterval(intervalHandle);
        }
    }, 10000);
}

//Checks whether the app is running inside a snap
function isSnap() {
    return typeof process.env.SNAP_NAME !== 'undefined' && process.env.SNAP_NAME !== 'node';
}

// Call main function
main();