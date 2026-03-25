/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;
using Samples.Datalayer.Provider;
using System;
using System.Threading.Tasks;

// Please define the node root folder in ctrlX Data Layer
const string root = "sdk/net/provider/subscription";

// Create TaskCompletionSource to wait for process termination.  
var tcs = new TaskCompletionSource();

// Handle process exit event (SIGTERM).
AppDomain.CurrentDomain.ProcessExit += (_, _) =>
{
    Console.WriteLine("Received 'SIGTERM' event.");

    // Run task for graceful shutdown.
    tcs.SetResult();
};

// Create a new ctrlX Data Layer system
using var system = new DatalayerSystem();

// Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX CORE.
system.Start(startBroker: false);
Console.WriteLine("ctrlX Data Layer system started.");

// Create a remote address with the parameters according to your environment
var remote = new Remote(ip: "192.168.1.1", sslPort: 443).ToString();

// Create a Datalayer Provider instance and connect. Automatically reconnects if the connection is interrupted.
using var provider = system.Factory.CreateProvider(remote);
Console.WriteLine("ctrlX Data Layer provider created.");

// Create and register node on given address.
DLR_RESULT result;
var node = Node.CreateReadWriteNode($"{root}/myInt", "types/datalayer/int32", "Description here", new Variant(42));
var nodeHandler = new SubscriptionNodeHandler(node);
(result, _) = provider.RegisterNode(node.Address, nodeHandler);
Console.WriteLine($"Registering Node with address='{node.Address}', result='{result}'");

// Start the Provider
result = provider.Start();

// Check if provider is started.
Console.WriteLine(value: $"Provider started: {result}");

// Check if provider is connected.
if (!provider.IsConnected)
{
    // Initially exit and retry after app restart-delay (see snapcraft.yaml).
    Console.WriteLine($"Provider is not connected -> exit");
    return;
}

// Start publishing.
nodeHandler.StartPublish();

// Wait for process termination.
Console.WriteLine("Waiting for process exit event 'SIGTERM'...");
await tcs.Task;
Console.WriteLine("Graceful shutdown.");

// Stop publishing.
nodeHandler.StopPublish();

// Stop the provider.
provider.Stop();
Console.WriteLine("Provider stopped.");

// Stop the ctrlX Data Layer system.
system.Stop();
Console.WriteLine("ctrlX Data Layer system stopped.");