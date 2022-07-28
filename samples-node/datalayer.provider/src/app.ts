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

import DatalayerSystem from 'ctrlx-datalayer/dist/datalayersystem';
import Result from 'ctrlx-datalayer/dist/result';
import ProviderNodeCallbacks from 'ctrlx-datalayer/dist/providernodecallbacks';
import IProviderNodeResult from "ctrlx-datalayer/dist/providernoderesult";
import IVariant from "ctrlx-datalayer/dist/variant.js";

// Flatbuffers
import * as flatbuffers from 'flatbuffers';
import { InertialValue } from './sampleSchema';

// MetadataUtils
import * as MetadataUtils from 'ctrlx-datalayer/dist/metadata-utils';
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

    // Create DTO with address, value and metadata
    const myInt = {
        address: 'samples/nodejs/myInt',
        value: 123,
        metadata: new MetadataUtils.MetadataBuilder(MetadataUtils.AllowedOperationFlags.Read, "myInt Description", "http://myInt/description/url")
            .setDisplayName("myInt DisplayName")
            .setUnit("myInt Unit")
            .setNodeClass(MetadataUtils.NodeClass.Variable)
            .addReference(MetadataUtils.ReferenceType.ReadType, "types/datalayer/int32")
            .build()
    }

    // Create DTO with address, value and metadata
    const myDouble = {
        address: 'samples/nodejs/myDouble',
        value: Math.PI,
        metadata: new MetadataUtils.MetadataBuilder(MetadataUtils.AllowedOperationFlags.Read, "myDouble Description", "http://myDouble/description/url")
            .setDisplayName("myDouble DisplayName")
            .setUnit("myDouble Unit")
            .setNodeClass(MetadataUtils.NodeClass.Variable)
            .addReference(MetadataUtils.ReferenceType.ReadType, "types/datalayer/float64")
            .build()
    }

    // Create DTO with address, value and metadata
    const myString = {
        address: 'samples/nodejs/myString',
        value: 'Hello ctrlX',
        metadata: new MetadataUtils.MetadataBuilder(MetadataUtils.AllowedOperationFlags.Read | MetadataUtils.AllowedOperationFlags.Write, "myString Description", "http://myString/description/url")
            .setDisplayName("myString DisplayName")
            .setUnit("myString Unit")
            .setNodeClass(MetadataUtils.NodeClass.Variable)
            .addReference(MetadataUtils.ReferenceType.ReadType, "types/datalayer/string")
            .addReference(MetadataUtils.ReferenceType.WriteType, "types/datalayer/string")
            .build()
    }

    // Create DTO with address, value and metadata
    const myFlatbuffer = {
        address: 'samples/nodejs/myFlatbuffer',
        value: (() => {
            const builder = new flatbuffers.Builder(DatalayerSystem.defaultFlatbuffersInitialSize);
            const offset = InertialValue.createInertialValue(builder, 30, -442, 911);
            builder.finish(offset);
            return builder.asUint8Array();
        })(),
        metadata: new MetadataUtils.MetadataBuilder(MetadataUtils.AllowedOperationFlags.Read, "myFlatbuffer Description", "http://myFlatbuffer/description/url")
            .setDisplayName("myFlatbuffer DisplayName")
            .setUnit("myFlatbuffer Unit")
            .setNodeClass(MetadataUtils.NodeClass.Variable)
            .addReference(MetadataUtils.ReferenceType.ReadType, typeAddressInertialValue)
            .build()
    }

    // Implement onMetadata callback function
    const onMetadata = (address: string, result: IProviderNodeResult) => {
        console.log('onMetadata', address);
        switch (address) {
            case myInt.address:
                result.setResult(Result.OK, myInt.metadata, 'flatbuffers');
                break;
            case myDouble.address:
                result.setResult(Result.OK, myDouble.metadata, 'flatbuffers');
                break;
            case myString.address:
                result.setResult(Result.OK, myString.metadata, 'flatbuffers');
                break;
            case myFlatbuffer.address:
                result.setResult(Result.OK, myFlatbuffer.metadata, 'flatbuffers');
                break;
            default:
                result.setResult(Result.UNSUPPORTED, null);
                break;
        }
    };

    // Implement onRead callback function
    const onRead = (address: string, args: IVariant, result: IProviderNodeResult) => {
        console.log('onRead', address, args);
        switch (address) {
            case myInt.address:
                result.setResult(Result.OK, myInt.value);
                break;
            case myDouble.address:
                result.setResult(Result.OK, myDouble.value);
                break;
            case myString.address:
                result.setResult(Result.OK, myString.value);
                break;
            case myFlatbuffer.address:
                result.setResult(Result.OK, myFlatbuffer.value, 'flatbuffers');
                break;
            default:
                result.setResult(Result.UNSUPPORTED, null);
                break;
        }
    };

    // Implement onWrite callback function
    const onWrite = (address: string, args: IVariant, result: IProviderNodeResult) => {
        console.log('onWrite', address, args);
        switch (address) {
            case myString.address:
                myString.value = args.value;
                result.setResult(Result.OK, myString.value);
                break;
            default:
                result.setResult(Result.UNSUPPORTED, null);
                break;
        }
    };

    const providerNodeCallbacks = new ProviderNodeCallbacks()
        .setOnMetadata(onMetadata)
        .setOnRead(onRead)
        .setOnWrite(onWrite);

    // Register the nodes we wan't to provide
    await provider.registerNode(myInt.address, providerNodeCallbacks).then((result) => console.log('registered node', myInt.address, 'with result:', result))
    await provider.registerNode(myDouble.address, providerNodeCallbacks).then((result) => console.log('registered node', myDouble.address, 'with result:', result));
    await provider.registerNode(myString.address, providerNodeCallbacks).then((result) => console.log('registered node', myString.address, 'with result:', result));
    await provider.registerNode(myFlatbuffer.address, providerNodeCallbacks).then((result) => console.log('registered node', myFlatbuffer.address, 'with result:', result));

    //Keep the process alive until disconnected
    const intervalHandle = setInterval(() => {
        if (system.isStarted() === false || provider.isConnected() === false) {
            clearInterval(intervalHandle);
        }
    }, 10000);
};

//Checks whether the app is running inside a snap
function isSnap() {
    return typeof process.env.SNAP_NAME !== 'undefined' && process.env.SNAP_NAME !== 'node';
}

// Call main function
main();