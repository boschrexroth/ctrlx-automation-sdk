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

import Browser from './browser';

// The main function
async function main() {



    // Create a new ctrlX Data Layer system
    const system = new DatalayerSystem('');

    // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
    await system.start(false);

    // Create a connection string with the parameters according to your environment (see DatalayerHelper class)
    const connectionString = DatalayerHelper.getConnectionString({ ip: "192.168.1.1", sslPort: 443 })
    console.log('connection string:', connectionString)

    // Create the client with the given remote address
    const client = await system.createClient(connectionString);

    // Check if client is connected.
    if (client.isConnected() === false) {
        console.log('client is not connected -> exit.')
        return;
    }

    // Create a Data Layer browser.
    const browser = new Browser(client);

    // Traverse the complete Data Layer tree.
    await browser.traverse();

    // Stop the Data Layer system.
    await system.stop();
}

//Checks whether the app is running inside a snap
function isSnap() {
    return typeof process.env.SNAP_NAME !== 'undefined' && process.env.SNAP_NAME !== 'node';
}

// Call main function
main();
