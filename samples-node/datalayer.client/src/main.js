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

const flatbuffers = require('flatbuffers');
const DatalayerSystem = require('ctrlx-datalayer-nodejs/dist/datalayersystem.js');
const notify_info = require('ctrlx-datalayer-nodejs/fbs/js/notify_info_generated');
const sub_properties = require('ctrlx-datalayer-nodejs/fbs/js/sub_properties_generated');

// The main function
async function main() {

    // This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:PORT
    // Please check and change according your environment:
    // - USER:       Enter your user name here - default is boschrexroth
    // - PASSWORD:   Enter your password here - default is boschrexroth
    // - IP_ADDRESS: 127.0.0.1   If you develop in WSL and you want to connect to a ctrlX CORE virtual with port forwarding
    //               10.0.2.2    If you develop in a VM (Virtual Box, QEMU,...) and you want to connect to a ctrlX virtual with port forwarding
    //               192.168.1.1 If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater
    const connectionTcp = 'tcp://boschrexroth:boschrexroth@10.0.2.2:2069';

    // Please use the connection string for Inter Process Communication
    // for performance reasons when running inside a snap:
    const connectionIpc = DatalayerSystem.protocolSchemeIpc;

    // Check if the process is running inside a snap 
    const isSnapped = process.env.SNAP_NAME === "ctrlx-node-datalayer-client";
    console.log("running inside snap:", isSnapped)

    // Create a new ctrlX Data Layer system
    const system = new DatalayerSystem('');

    // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
    await system.start(false);

    // Set the remote address to inter-process communication (ipc) protocol if running in snap, otherwise tcp
    const remote = isSnapped ? connectionIpc : connectionTcp;
    console.log("client remote address:", remote)

    // Create the client with the given remote address
    const client = await system.createClient(remote);

    if (client.isConnected() === false) {
        console.log("client is not connected -> exit.")
        return;
    }
   
    // Read value from datalayer and log to console
    const addressMemUsedPercent = 'framework/metrics/system/memused-percent';
    const valueMemUsed = await client.read(addressMemUsedPercent);
    console.log("read from:", addressMemUsedPercent)
    console.log(valueMemUsed.timestamp, addressMemUsedPercent, valueMemUsed.value)

    // Create a datachanged event callback function
    const dataChanged = (item) => {
        // Fetch NotifyInfo, which is a flatbuffers holding the timestamp (filetime nanos), notifyType and the source node
        const bb = new flatbuffers.ByteBuffer(item.info);
        const info = notify_info.comm.datalayer.NotifyInfo.getRootAsNotifyInfo(bb);

        // Convert the timestamp (filetime nanos since 1601 UTC) to date (milliseconds since 1 January 1970 UTC)
        const timestamp = DatalayerSystem.toDate(info.timestamp().toFloat64());

        console.log(timestamp, info.node(), item.value, item.type);

        // console.log(item)
    }

    // Setup subscription properties
    const propertiesBuilder = new flatbuffers.Builder(DatalayerSystem.defaultFlatbuffersInitialSize);
    const id = propertiesBuilder.createString('mySubscription');
    sub_properties.comm.datalayer.SubscriptionProperties.startSubscriptionProperties(propertiesBuilder);
    sub_properties.comm.datalayer.SubscriptionProperties.addId(propertiesBuilder, id);
    sub_properties.comm.datalayer.SubscriptionProperties.addKeepaliveInterval(propertiesBuilder, 10000);
    sub_properties.comm.datalayer.SubscriptionProperties.addPublishInterval(propertiesBuilder, 1000);
    sub_properties.comm.datalayer.SubscriptionProperties.addErrorInterval(propertiesBuilder, 1000);
    const offset = sub_properties.comm.datalayer.SubscriptionProperties.endSubscriptionProperties(propertiesBuilder);
    sub_properties.comm.datalayer.SubscriptionProperties.finishSubscriptionPropertiesBuffer(propertiesBuilder, offset);

    // Create subscription
    const subscription = await client.createSubscription(propertiesBuilder.asUint8Array(), dataChanged);

    // Subscribe a node
    const addressCpuUtilisationPercent = 'framework/metrics/system/cpu-utilisation-percent';
    console.log("subscribe to:", addressCpuUtilisationPercent)
    subscription.subscribe(addressCpuUtilisationPercent);

    // We stop the subscription after 5 min.
    const delay = 5 * 60 * 1000;
    setTimeout(async () => {
        await system.stop();
    }, delay);
};

// Call main function
main();