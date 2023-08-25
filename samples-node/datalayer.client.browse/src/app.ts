/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

import DatalayerSystem from 'ctrlx-datalayer/dist/datalayersystem';
import { Remote } from 'ctrlx-datalayer/dist/remote';

import Browser from './browser';

// The main function
async function main() {

    // Create a new ctrlX Data Layer system
    const system = new DatalayerSystem('');

    // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX CORE
    await system.start(false);

    // Create a remote address with the parameters according to your environment
    const remote = Remote.build({ ip: "192.168.1.1", sslPort: 443 })
    console.log('connection string:', remote)

    // Create the client with the given remote address
    const client = await system.createClient(remote);

    // Check if client is connected.
    if (client.isConnected() === false) {
        console.log('client is not connected -> exit.')
        return;
    }

    // Create a ctrlX Data Layer browser.
    const browser = new Browser(client);

    // Traverse the complete ctrlX Data Layer tree.
    await browser.traverse();

    // Stop the ctrlX Data Layer system.
    await system.stop();
}

// Call main function
main();
