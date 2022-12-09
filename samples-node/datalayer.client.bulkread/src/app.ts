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
import DatalayerHelper from './datalayerhelper';

// The main function
async function main() {

    // Create a new ctrlX Data Layer system
    const system = new DatalayerSystem('');

    // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
    await system.start(false);

    // Create a connection string with the parameters according to your environment (see DatalayerHelper class)
    const connectionString = DatalayerHelper.getConnectionString({ ip: "10.0.2.2", sslPort: 8443 })
    console.log('connection string:', connectionString)

    // Create the client with the given remote address
    const client = await system.createClient(connectionString);

    if (client.isConnected() === false) {
        console.log('client is not connected -> exit.')
        return;
    }

    // Define node addresss for bulk read
    const addresses = [
        "framework/metrics/system/cpu-utilisation-percent",
        "framework/metrics/system/memavailable-mb",
        "framework/metrics/system/membuffers-mb",
        "framework/metrics/system/memcache-mb",
        "framework/metrics/system/memfree-mb",
        "framework/metrics/system/memtotal-mb",
        "framework/metrics/system/memused-percent"
    ];

    // Call asynchronous bulk read 
    const items = await client.bulkRead(addresses);

    // Dump results of bulk read to console
    items.forEach(item => {
        console.log(`address: ${item.address}, value: ${item.value}, timestamp: ${item.timestamp.toISOString()}, result: ${item.result.text}`);
    });

    // Keep the process alive until disconnected
    const intervalHandle = setInterval(() => {
        if (system.isStarted() === false || client.isConnected() === false) {
            clearInterval(intervalHandle);
        }
    }, 10000);
}

// Call main function
main();
