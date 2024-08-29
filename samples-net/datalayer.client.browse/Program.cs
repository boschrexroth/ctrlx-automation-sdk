/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;
using Samples.Datalayer.Client.Browse;
using System;
using System.Threading.Tasks;

// Create TaskCompletionSource to wait for process termination  
var tcs = new TaskCompletionSource();

// Handle process exit event (SIGTERM)
AppDomain.CurrentDomain.ProcessExit += (_, _) =>
{
    Console.WriteLine("Received 'SIGTERM' event.");

    // Run task for graceful shutdown
    tcs.SetResult();
};

// Create a new ctrlX Data Layer system
using var system = new DatalayerSystem();

// Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX CORE
system.Start(startBroker: false);
Console.WriteLine("ctrlX Data Layer system started.");

// Create a remote address with the parameters according to your environment
var remote = new Remote(ip: "192.168.1.1", sslPort: 443).ToString();

// Create a Datalayer Client instance and connect. Automatically reconnects if the connection is interrupted.
using var client = system.Factory.CreateClient(remote);
Console.WriteLine("ctrlX Data Layer client created.");

// Traverse the whole ctrlX Data Layer tree
client.BrowseTree();

// Wait for process termination
Console.WriteLine("Waiting for process exit event 'SIGTERM' ...");
await tcs.Task;
Console.WriteLine("Gracefully shutdown app");

// Stop the ctrlX Data Layer system
system.Stop();
Console.WriteLine("ctrlX Data Layer system stopped.");