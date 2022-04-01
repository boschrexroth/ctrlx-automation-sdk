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

import Browser from './browser';

// The main function
async function main() {

    // This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:PORT
    // Please check and change according your environment:
    // - USER:       Enter your user name here - default is boschrexroth
    // - PASSWORD:   Enter your password here - default is boschrexroth
    // - IP_ADDRESS: 127.0.0.1   If you develop on your (Windows) host and you want to connect to a ctrlX CORE virtual with port forwarding
    //               10.0.2.2    If you develop on a VM (QEMU, Virtual Box) and you want to connect to a ctrlX virtual with port forwarding
    //               192.168.1.1 If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater
    const connectionTcp = 'tcp://boschrexroth:boschrexroth@10.0.2.2';

    // Please use the connection string for Inter Process Communication
    // for performance reasons when running inside a snap:
    const connectionIpc = DatalayerSystem.protocolSchemeIpc;

    // Check if the process is running inside a snap 
    const isSnapped = isSnap();
    console.log('running inside snap:', isSnapped)

    // Create a new ctrlX Data Layer system
    const system = new DatalayerSystem('');

    // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
    await system.start(false);

    // Set the remote address to inter-process communication (ipc) protocol if running in snap, otherwise tcp
    const remote = isSnapped ? connectionIpc : connectionTcp;
    console.log('client remote address:', remote)

    // Create the client with the given remote address
    const client = await system.createClient(remote);

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
