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
import DatalayerHelper from './datalayerhelper';


// The main function
async function main() {


    // Create a new ctrlX Data Layer system
    const system = new DatalayerSystem('');

    // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
    await system.start(false);

    // Create a connection string with the parameters according to your environment (see DatalayerHelper class)
    const connectionString = DatalayerHelper.getConnectionString({ ip: "192.168.1.1", sslPort: 443 })
    console.log('connection string:', connectionString)

    //Create a Provider with the given remote address
    const provider = await system.createProvider(connectionString);
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

    const now = new Date();

    // Create all static nodes.
    const staticNodes = [
        Node.createStatic(DataTypes.bool8, DataRanges.bool8.getMax()),
        Node.createStatic(DataTypes.int8, DataRanges.int8.getMax()),
        Node.createStatic(DataTypes.uint8, DataRanges.uint8.getMax()),
        Node.createStatic(DataTypes.int16, DataRanges.int16.getMax()),
        Node.createStatic(DataTypes.uint16, DataRanges.uint16.getMax()),
        Node.createStatic(DataTypes.int32, DataRanges.int32.getMax()),
        Node.createStatic(DataTypes.uint32, DataRanges.uint32.getMax()),
        Node.createStatic(DataTypes.int64, DataRanges.int64.getMax()),
        Node.createStatic(DataTypes.uint64, DataRanges.uint64.getMax()),
        Node.createStatic(DataTypes.float, DataRanges.float.getMax()),
        Node.createStatic(DataTypes.double, DataRanges.double.getMax()),
        Node.createStatic(DataTypes.string, 'String_0'),
        Node.createStatic(DataTypes.timestamp, now),
        Node.createStatic(DataTypes.inertialValue, inertialValue),
        Node.createStatic(DataTypes.arbool8, [true, false, true]),
        Node.createStatic(DataTypes.arint8, [DataRanges.int8.getMin(), -1, 0, DataRanges.int8.getMax()]),
        Node.createStatic(DataTypes.aruint8, [DataRanges.uint8.getMin(), DataRanges.uint8.getMax()]),
        Node.createStatic(DataTypes.arint16, [DataRanges.int16.getMin(), -1, 0, DataRanges.int16.getMax()]),
        Node.createStatic(DataTypes.aruint16, [DataRanges.uint16.getMin(), DataRanges.uint16.getMax()]),
        Node.createStatic(DataTypes.arint32, [DataRanges.int32.getMin(), -1, 0, DataRanges.int32.getMax()]),
        Node.createStatic(DataTypes.aruint32, [DataRanges.uint32.getMin(), DataRanges.uint32.getMax()]),
        Node.createStatic(DataTypes.arint64, [DataRanges.int64.getMin(), -1n, 0n, DataRanges.int64.getMax()]),
        Node.createStatic(DataTypes.aruint64, [DataRanges.uint64.getMin(), DataRanges.uint64.getMax()]),
        Node.createStatic(DataTypes.arfloat, [DataRanges.float.getMin(), -1.0, 0.0, DataRanges.float.getMax()]),
        Node.createStatic(DataTypes.ardouble, [DataRanges.double.getMin(), -1.0, 0.0, DataRanges.double.getMax()]),
        Node.createStatic(DataTypes.arstring, ['Blue', 'Red', 'Orange']),
        Node.createStatic(DataTypes.artimestamp, [now, now, now])
    ]

    // Create all dynamic nodes.
    const dynamicNodes = [
        Node.createDynamic(DataTypes.bool8, 0),
        Node.createDynamic(DataTypes.int8, 0),
        Node.createDynamic(DataTypes.uint8, 0),
        Node.createDynamic(DataTypes.int16, 0),
        Node.createDynamic(DataTypes.uint16, 0),
        Node.createDynamic(DataTypes.int32, 0),
        Node.createDynamic(DataTypes.uint32, 0),
        Node.createDynamic(DataTypes.int64, 0n),
        Node.createDynamic(DataTypes.uint64, 0n),
        Node.createDynamic(DataTypes.float, 0),
        Node.createDynamic(DataTypes.double, 0),
        Node.createDynamic(DataTypes.string, 'String_0'),
        Node.createDynamic(DataTypes.timestamp, new Date()),
        Node.createDynamic(DataTypes.inertialValue, inertialValue),
        Node.createDynamic(DataTypes.arbool8, [true, false, true, false, true, false, true, false, true, false]),
        Node.createDynamic(DataTypes.arint8, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(DataTypes.aruint8, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(DataTypes.arint16, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(DataTypes.aruint16, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(DataTypes.arint32, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(DataTypes.aruint32, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(DataTypes.arint64, [-5n, -4n, -3n, -2n, -1n, 0n, 1n, 2n, 3n, 4n]),
        Node.createDynamic(DataTypes.aruint64, [0n, 1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n]),
        Node.createDynamic(DataTypes.arfloat, [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]),
        Node.createDynamic(DataTypes.ardouble, [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]),
        Node.createDynamic(DataTypes.arstring, ['String_0', 'String_1', 'String_2', 'String_3', 'String_4', 'String_5', 'String_6', 'String_7', 'String_8', 'String_9']),
        Node.createDynamic(DataTypes.artimestamp, [now, now, now, now, now, now, now, now])
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
            const now = new Date();

            dynamicNodes.forEach(async node => {
                node.incrementValue(now);
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