#!/usr/bin/env node
// Copyright (c) 2021 Bosch Rexroth AG

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

const DatalayerSystem = require('ctrlx-datalayer-nodejs/dist/datalayersystem.js');
const Result = require('ctrlx-datalayer-nodejs/dist/result');
const MetadataBuilder = require("./metadatabuilder");
const sampleSchema_generated = require("./sampleSchema_generated");
const flatbuffers = require('flatbuffers');

// The main function
async function main() {

    // This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:PORT
    // Please check and change according your environment:
    // - USER:       Enter your user name here - default is boschrexroth
    // - PASSWORD:   Enter your password here - default is boschrexroth
    // - IP_ADDRESS: 127.0.0.1   If you develop in WSL and you want to connect to a ctrlX CORE virtual with port forwarding
    //               10.0.2.2    If you develop in a VM (Virtual Box, QEMU,...) and you want to connect to a ctrlX virtual with port forwarding
    //               192.168.1.1 If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater
    const connectionTcp = 'tcp://boschrexroth:boschrexroth@10.0.2.2:2070';

    // Please use the connection string for Inter Process Communication
    // for performance reasons when running inside a snap:
    const connectionIpc = DatalayerSystem.protocolSchemeIpc; 

    // Check if the process is running inside a snap 
    const isSnapped = process.env.SNAP_NAME === "ctrlx-node-datalayer-provider";
    console.log("running inside snap:", isSnapped)

    // Create a new ctrlX Data Layer system
    const system = new DatalayerSystem('');

    // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
    await system.start(false);

    // Set the remote address to inter-process communication (ipc) protocol if running in snap, otherwise tcp
    const remote = isSnapped ? connectionIpc : connectionTcp;
    console.log("provider remote address:", remote)

    //Create a Provider with the given remote address
    const provider = await system.createProvider(remote);
    await provider.start();

    if (provider.isConnected() === false) {
        console.log("provider is not connected -> exit.")
        return;
    }

    // Register type with binary flatbuffers schema file: sampleSchema.bfbs (auto generated from sampleSchema.fbs by flatc compiler)
    const typeAddressInertialValue = "types/samples/nodejs/schema/inertial-value";
    const sampleSchemaFile = __dirname + "/sampleSchema.bfbs";
    await provider.registerType(typeAddressInertialValue, sampleSchemaFile).then(result => {
        console.log("Registering Type with address=", typeAddressInertialValue, ", result=", result);
    });

    // Create DTO with address, value and metadata
    const myInt = {
        address: 'samples/nodejs/myInt',
        value: 123,
        metadata: new MetadataBuilder()
            .setDescription("myInt Description")
            .setDescriptionUrl("http://myInt/description/url")
            .setDisplayName("myInt DisplayName")
            .setUnit("myInt Unit")
            .setNodeClass("Variable")
            .setAllowedOperations(true, false, false, false, false, false)
            .addReference("readType", "types/datalayer/int32")
            .build()
    }

    // Create DTO with address, value and metadata
    const myDouble = {
        address: 'samples/nodejs/myDouble',
        value: Math.PI,
        metadata: new MetadataBuilder()
            .setDescription("myDouble Description")
            .setDescriptionUrl("http://myDouble/description/url")
            .setDisplayName("myDouble DisplayName")
            .setUnit("myDouble Unit")
            .setNodeClass("Variable")
            .setAllowedOperations(true, false, false, false, false, false)
            .addReference("readType", "types/datalayer/float64")
            .build()
    }

    // Create DTO with address, value and metadata
    const myString = {
        address: 'samples/nodejs/myString',
        value: 'Hello ctrlX',
        metadata: new MetadataBuilder()
            .setDescription("myString Description")
            .setDescriptionUrl("http://myString/description/url")
            .setDisplayName("myString DisplayName")
            .setUnit("myString Unit")
            .setNodeClass("Variable")
            .setAllowedOperations(true, true, false, false, false, false)
            .addReference("readType", "types/datalayer/string")
            .addReference("writeType", "types/datalayer/string")
            .build()
    }

    // Create DTO with address, value and metadata
    const myFlatbuffer = {
        address: 'samples/nodejs/myFlatbuffer',
        value: (() => {
            builder = new flatbuffers.Builder(DatalayerSystem.defaultFlatbuffersInitialSize);
            const offset = sampleSchema_generated.sample.schema.InertialValue.createInertialValue(builder, 30, -442, 911);
            builder.finish(offset);
            return builder.asUint8Array();
        })(),
        metadata: new MetadataBuilder()
            .setDescription("myFlatbuffer Description")
            .setDescriptionUrl("http://myFlatbuffer/description/url")
            .setDisplayName("myFlatbuffer DisplayName")
            .setUnit("myFlatbuffer Unit")
            .setNodeClass("Variable")
            .setAllowedOperations(true, false, false, false, false, false)
            .addReference("readType", typeAddressInertialValue)
            .build()
    }

    // Implement onCreate callback function
    const onCreate = (address, args, result) => {
        console.log('onCreate', address, args);
        result.setResult(Result.UNSUPPORTED, null);
    };

    // Implement onRemove callback function
    const onRemove = (address, result) => {
        console.log('onRemove', address);
        result.setResult(Result.UNSUPPORTED, null);
    };

    // Implement onBrowse callback function
    const onBrowse = (address, result) => {
        console.log('onBrowse', address);
        result.setResult(Result.UNSUPPORTED, null);
    };

    // Implement onMetadata callback function
    const onMetadata = (address, result) => {
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
    const onRead = (address, args, result) => {
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
    const onWrite = (address, args, result) => {
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

    // Register the nodes we wan't to provide
    await provider.registerNode(myInt.address, onCreate, onRemove, onBrowse, onMetadata, onRead, onWrite).then((result) => console.log("registered node", myInt.address, "with result:", result))
    await provider.registerNode(myDouble.address, onCreate, onRemove, onBrowse, onMetadata, onRead, onWrite).then((result) => console.log("registered node", myDouble.address, "with result:", result));
    await provider.registerNode(myString.address, onCreate, onRemove, onBrowse, onMetadata, onRead, onWrite ).then((result) => console.log("registered node", myString.address, "with result:", result));
    await provider.registerNode(myFlatbuffer.address, onCreate, onRemove, onBrowse, onMetadata, onRead, onWrite).then((result) => console.log("registered node", myFlatbuffer.address, "with result:", result));

    //Keep the provider alive for 5 min.
    const delay = 5 * 60 * 1000;
    setTimeout(async () => {
        await system.stop();
    }, delay );
};

// Call main function
main();