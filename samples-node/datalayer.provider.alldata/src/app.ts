#!/usr/bin/env node
// !!! DO NOT REMOVE THE SHEBANG ON TOP OF THE FILE, WHICH SPECIFIES THIS APP TO BE EXECUTED BY NODE.JS !!!

/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

// DatalayerSystem
import DatalayerSystem from 'ctrlx-datalayer/dist/datalayersystem';
import Result from 'ctrlx-datalayer/dist/result';
import ProviderNodeCallbacks from 'ctrlx-datalayer/dist/providernodecallbacks';
import IVariant from 'ctrlx-datalayer/dist/variant';
import IProviderNodeResult from 'ctrlx-datalayer/dist/providernoderesult';

// Flatbuffers
import * as flatbuffers from 'flatbuffers';
import { InertialValueT } from './sample/schema/inertial-value';

import DataType from './datatype';
import DataRanges from './dataranges';
import DataTypes from './datatypes';
import Node from './node';

// Utils
import { Remote } from 'ctrlx-datalayer/dist/remote';

// The main function
async function main() {

    // Create a new ctrlX Data Layer system
    const system = new DatalayerSystem('');

    // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX CORE
    await system.start(false);

    // Create a remote address with the parameters according to your environment
    const remote = Remote.build({ ip: "10.0.2.2", sslPort: 8443 });
    console.log('connection string:', remote);

    //Create a Provider with the given remote address
    const provider = await system.createProvider(remote);
    await provider.start();

    if (provider.isConnected() === false) {
        console.log('provider is not connected -> exit.');
        return;
    }

    // Register type with binary flatbuffers schema file: sampleSchema.bfbs (auto generated from sampleSchema.fbs by flatc compiler)
    const typeAddressInertialValue = `${DataType.rootType}/inertial-value`;
    const sampleSchemaFile = __dirname + '/bfbs/sampleSchema.bfbs';
    await provider.registerType(typeAddressInertialValue, sampleSchemaFile).then(result => {
        console.log('Registering Type with address=', typeAddressInertialValue, ', result=', result);
    });

    const builder = new flatbuffers.Builder(DatalayerSystem.defaultFlatbuffersInitialSize);
    const inertialValueT = new InertialValueT(30, -442, 911);
    const offset = inertialValueT.pack(builder);
    builder.finish(offset);
    const inertialValue = builder.asUint8Array();

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
        Node.createStatic(DataTypes.arrayOfBool8, [true, false, true]),
        Node.createStatic(DataTypes.arrayOfInt8, [DataRanges.int8.getMin(), -1, 0, DataRanges.int8.getMax()]),
        Node.createStatic(DataTypes.arrayOfUint8, [DataRanges.uint8.getMin(), DataRanges.uint8.getMax()]),
        Node.createStatic(DataTypes.arrayOfInt16, [DataRanges.int16.getMin(), -1, 0, DataRanges.int16.getMax()]),
        Node.createStatic(DataTypes.arrayOfUint16, [DataRanges.uint16.getMin(), DataRanges.uint16.getMax()]),
        Node.createStatic(DataTypes.arrayOfInt32, [DataRanges.int32.getMin(), -1, 0, DataRanges.int32.getMax()]),
        Node.createStatic(DataTypes.arrayOfUint32, [DataRanges.uint32.getMin(), DataRanges.uint32.getMax()]),
        Node.createStatic(DataTypes.arrayOfInt64, [DataRanges.int64.getMin(), -1n, 0n, DataRanges.int64.getMax()]),
        Node.createStatic(DataTypes.arrayOfUint64, [DataRanges.uint64.getMin(), DataRanges.uint64.getMax()]),
        Node.createStatic(DataTypes.arrayOfFloat, [DataRanges.float.getMin(), -1.0, 0.0, DataRanges.float.getMax()]),
        Node.createStatic(DataTypes.arrayOfFloat64, [DataRanges.double.getMin(), -1.0, 0.0, DataRanges.double.getMax()]),
        Node.createStatic(DataTypes.arrayOfString, ['Blue', 'Red', 'Orange']),
        Node.createStatic(DataTypes.arrayOfTimestamp, [now, now, now])
    ];

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
        Node.createDynamic(DataTypes.arrayOfBool8, [true, false, true, false, true, false, true, false, true, false]),
        Node.createDynamic(DataTypes.arrayOfInt8, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(DataTypes.arrayOfUint8, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(DataTypes.arrayOfInt16, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(DataTypes.arrayOfUint16, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(DataTypes.arrayOfInt32, [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4]),
        Node.createDynamic(DataTypes.arrayOfUint32, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
        Node.createDynamic(DataTypes.arrayOfInt64, [-5n, -4n, -3n, -2n, -1n, 0n, 1n, 2n, 3n, 4n]),
        Node.createDynamic(DataTypes.arrayOfUint64, [0n, 1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n]),
        Node.createDynamic(DataTypes.arrayOfFloat, [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]),
        Node.createDynamic(DataTypes.arrayOfFloat64, [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]),
        Node.createDynamic(DataTypes.arrayOfString, ['String_0', 'String_1', 'String_2', 'String_3', 'String_4', 'String_5', 'String_6', 'String_7', 'String_8', 'String_9']),
        Node.createDynamic(DataTypes.arrayOfTimestamp, [now, now, now, now, now, now, now, now])
    ];

    // Create map with key address and value node for the callback functions.
    let addressToNode = new Map<string, Node>();

    // Add all static nodes to the map.
    staticNodes.forEach(node => {
        addressToNode.set(node.getAddress(), node);
    });

    // Add all dynamic nodes to the map.
    dynamicNodes.forEach(node => {
        addressToNode.set(node.getAddress(), node);
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
            (result) => console.log('registered static node', node.getAddress(), 'with result:', result));
    });

    // Register dynamic nodes
    dynamicNodes.forEach(async node => {
        await provider.registerNode(node.getAddress(), providerNodeCallbacks).then(
            (result) => console.log('registered dynamic node', node.getAddress(), 'with result:', result));
    });

    // Keep the process alive until disconnected
    const intervalHandle = setInterval(() => {
        dynamicNodes.forEach(async node => {
            node.incrementValue(new Date());
        });

        if (system.isStarted() === false || provider.isConnected() === false) {
            clearInterval(intervalHandle);
        }
    }, 1_000);


    // Handle process event 'SIGTERM'
    process.on('SIGTERM', () => {
        console.info('SIGTERM signal received.');

        system.stop();
        console.log('ctrlX Data Layer system stopped.');
    });
}

// Call main function
main();