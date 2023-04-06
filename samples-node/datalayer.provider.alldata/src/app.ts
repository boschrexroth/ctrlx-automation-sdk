#!/usr/bin/env node
// !!! DO NOT REMOVE THE SHEBANG ON TOP OF THE FILE, WHICH SPECIFIES THIS APP TO BE EXECUTED BY NODE.JS !!!

// Copyright (c) 2021-2023 Bosch Rexroth AG

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

import DATA_RANGES from './dataranges';
import DATA_TYPES from './datatypes';
import Node from './node';

// Utils
import { Remote } from 'ctrlx-datalayer/dist/remote';

// The main function
async function main() {


    // Create a new ctrlX Data Layer system
    const system = new DatalayerSystem('');

    // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
    await system.start(false);

    // Create a remote address with the parameters according to your environment
    const remote = Remote.build({ ip: "192.168.1.1", sslPort: 443 })
    console.log('connection string:', remote)

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
    const inertialValue = builder.asUint8Array();

    const now = new Date();

    // Create all static nodes.
    const staticNodes = [
        Node.createStatic(DATA_TYPES.bool8, DATA_RANGES.bool8.getMax()),
        Node.createStatic(DATA_TYPES.int8, DATA_RANGES.int8.getMax()),
        Node.createStatic(DATA_TYPES.uint8, DATA_RANGES.uint8.getMax()),
        Node.createStatic(DATA_TYPES.int16, DATA_RANGES.int16.getMax()),
        Node.createStatic(DATA_TYPES.uint16, DATA_RANGES.uint16.getMax()),
        Node.createStatic(DATA_TYPES.int32, DATA_RANGES.int32.getMax()),
        Node.createStatic(DATA_TYPES.uint32, DATA_RANGES.uint32.getMax()),
        Node.createStatic(DATA_TYPES.int64, DATA_RANGES.int64.getMax()),
        Node.createStatic(DATA_TYPES.uint64, DATA_RANGES.uint64.getMax()),
        Node.createStatic(DATA_TYPES.float, DATA_RANGES.float.getMax()),
        Node.createStatic(DATA_TYPES.double, DATA_RANGES.double.getMax()),
        Node.createStatic(DATA_TYPES.string, 'String_0'),
        Node.createStatic(DATA_TYPES.timestamp, now),
        Node.createStatic(DATA_TYPES.inertialValue, inertialValue),
        Node.createStatic(DATA_TYPES.arbool8, [true, false, true]),
        Node.createStatic(DATA_TYPES.arint8, [DATA_RANGES.int8.getMin(), -1, 0, DATA_RANGES.int8.getMax()]),
        Node.createStatic(DATA_TYPES.aruint8, [DATA_RANGES.uint8.getMin(), DATA_RANGES.uint8.getMax()]),
        Node.createStatic(DATA_TYPES.arint16, [DATA_RANGES.int16.getMin(), -1, 0, DATA_RANGES.int16.getMax()]),
        Node.createStatic(DATA_TYPES.aruint16, [DATA_RANGES.uint16.getMin(), DATA_RANGES.uint16.getMax()]),
        Node.createStatic(DATA_TYPES.arint32, [DATA_RANGES.int32.getMin(), -1, 0, DATA_RANGES.int32.getMax()]),
        Node.createStatic(DATA_TYPES.aruint32, [DATA_RANGES.uint32.getMin(), DATA_RANGES.uint32.getMax()]),
        Node.createStatic(DATA_TYPES.arint64, [DATA_RANGES.int64.getMin(), -1n, 0n, DATA_RANGES.int64.getMax()]),
        Node.createStatic(DATA_TYPES.aruint64, [DATA_RANGES.uint64.getMin(), DATA_RANGES.uint64.getMax()]),
        Node.createStatic(DATA_TYPES.arfloat, [DATA_RANGES.float.getMin(), -1.0, 0.0, DATA_RANGES.float.getMax()]),
        Node.createStatic(DATA_TYPES.ardouble, [DATA_RANGES.double.getMin(), -1.0, 0.0, DATA_RANGES.double.getMax()]),
        Node.createStatic(DATA_TYPES.arstring, ['Blue', 'Red', 'Orange']),
        Node.createStatic(DATA_TYPES.artimestamp, [now, now, now])
    ]

    // Create all dynamic nodes.
    const dynamicNodes = [
        Node.createDynamic(DATA_TYPES.bool8, 0),
        Node.createDynamic(DATA_TYPES.int8, 0),
        Node.createDynamic(DATA_TYPES.uint8, 0),
        Node.createDynamic(DATA_TYPES.int16, 0),
        Node.createDynamic(DATA_TYPES.uint16, 0),
        Node.createDynamic(DATA_TYPES.int32, 0),
        Node.createDynamic(DATA_TYPES.uint32, 0),
        Node.createDynamic(DATA_TYPES.int64, 0n),
        Node.createDynamic(DATA_TYPES.uint64, 0n),
        Node.createDynamic(DATA_TYPES.float, 0),
        Node.createDynamic(DATA_TYPES.double, 0),
        Node.createDynamic(DATA_TYPES.string, 'String_0'),
        Node.createDynamic(DATA_TYPES.timestamp, new Date()),
        Node.createDynamic(DATA_TYPES.inertialValue, inertialValue),
        Node.createDynamic(DATA_TYPES.arbool8, [true, false, true, false, true, false, true, false, true, false]),
        Node.createDynamic(DATA_TYPES.arint8, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(DATA_TYPES.aruint8, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(DATA_TYPES.arint16, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(DATA_TYPES.aruint16, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(DATA_TYPES.arint32, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(DATA_TYPES.aruint32, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(DATA_TYPES.arint64, [-5n, -4n, -3n, -2n, -1n, 0n, 1n, 2n, 3n, 4n]),
        Node.createDynamic(DATA_TYPES.aruint64, [0n, 1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n]),
        Node.createDynamic(DATA_TYPES.arfloat, [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]),
        Node.createDynamic(DATA_TYPES.ardouble, [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]),
        Node.createDynamic(DATA_TYPES.arstring, ['String_0', 'String_1', 'String_2', 'String_3', 'String_4', 'String_5', 'String_6', 'String_7', 'String_8', 'String_9']),
        Node.createDynamic(DATA_TYPES.artimestamp, [now, now, now, now, now, now, now, now])
    ]

    // Create map with key address and value node for the callback functions.
    let addressToNode = new Map<string, Node>()

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

// Call main function
main();