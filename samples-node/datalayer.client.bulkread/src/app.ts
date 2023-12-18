#!/usr/bin/env node
// !!! DO NOT REMOVE THE SHEBANG ON TOP OF THE FILE, WHICH SPECIFIES THIS APP TO BE EXECUTED BY NODE.JS !!!

/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

import DatalayerSystem from 'ctrlx-datalayer/dist/datalayersystem';
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

    // Create the client with the given remote address
    const client = await system.createClient(remote);

    // Define node addresses for bulk read
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

    //Keep the process alive until disconnected
    const intervalHandle = setInterval(() => {
        if (system.isStarted() === false || client.isConnected() === false) {
            clearInterval(intervalHandle);
        }
    }, 10_000);

    // Handle process event 'SIGTERM'
    process.on('SIGTERM', () => {
        console.info('SIGTERM signal received.');

        system.stop();
        console.log('ctrlX Data Layer system stopped.');
    });
}

// Call main function
main();
