/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;
using Google.FlatBuffers;
using sample.schema;
using Samples.Datalayer.Provider;
using System;
using System.IO;
using System.Threading.Tasks;

// Please define the node root folder in ctrlX Data Layer.
const string root = "sdk/net/provider/simple";

// Create TaskCompletionSource to wait for process termination.
var tcs = new TaskCompletionSource();

// Handle process exit event (SIGTERM).
AppDomain.CurrentDomain.ProcessExit += (_, _) =>
{
    Console.WriteLine("Received 'SIGTERM' event.");

    // Run task for graceful shutdown.
    tcs.SetResult();
};

// Create a new ctrlX Data Layer system.
using var system = new DatalayerSystem();

// Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX CORE.
system.Start(startBroker: false);
Console.WriteLine("ctrlX Data Layer system started.");

// Create a remote address with the parameters according to your environment.
var remote = new Remote(ip: "192.168.1.1", sslPort: 443).ToString();

// Create a Datalayer Provider instance and connect. Automatically reconnects if the connection is interrupted.
using var provider = system.Factory.CreateProvider(remote);
Console.WriteLine("ctrlX Data Layer provider created.");

DLR_RESULT result;

// Create and register some read-only nodes.
var intNode = Node.CreateReadOnlyNode($"{root}/myInt", "types/datalayer/int32", "Description here", new Variant(42));
(result, _) = provider.RegisterNode(intNode.Address, new ReadOnlyNodeHandler(intNode));
Console.WriteLine($"Registering Node with address='{intNode.Address}', result='{result}'");

var doubleNode = Node.CreateReadOnlyNode($"{root}/myDouble", "types/datalayer/float64", "Description here", new Variant(Math.PI));
(result, _) = provider.RegisterNode(doubleNode.Address, new ReadOnlyNodeHandler(doubleNode));
Console.WriteLine($"Registering Node with address='{doubleNode.Address}', result='{result}'");

// Create and register some read-write nodes.
var stringNode = Node.CreateReadWriteNode($"{root}/myString", "types/datalayer/string", "Description here", new Variant("Hello ctrlX"));
(result, _) = provider.RegisterNode(stringNode.Address, new ReadWriteNodeHandler(stringNode));
Console.WriteLine($"Registering Node with address='{stringNode.Address}', result='{result}'");

// Register type with binary flatbuffers schema file: sampleSchema.bfbs (auto generated from sampleSchema.fbs by flatc compiler).
const string typeAddressInertialValue = $"types/{root}/MyInertialValue";
result = provider.RegisterType(typeAddressInertialValue, Path.Combine(AppContext.BaseDirectory, "bfbs", "sampleSchema.bfbs"));
Console.WriteLine($"Registering Type with address='{typeAddressInertialValue}', result='{result}'");

// Create flatbuffers value with FlatbufferBuilder (flatbuffers .NET API) and InertialValue (auto generated from sampleSchema.fbs by flatc compiler) .
var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
var offset = InertialValue.CreateInertialValue(builder, 30, -442, 911);
builder.Finish(offset.Value);
var variantFlatbuffers = new Variant(builder);

// Create and register flatbuffers node read-only.
var fbsNode = Node.CreateReadOnlyNode($"{root}/inertial-value", typeAddressInertialValue, "Description here", variantFlatbuffers);
(result, _) = provider.RegisterNode(fbsNode.Address, new ReadOnlyNodeHandler(fbsNode));
Console.WriteLine($"Registering Node with address='{fbsNode.Address}', result='{result}'");

// Start the Provider.
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

// Wait for process termination.
Console.WriteLine("Waiting for process exit event 'SIGTERM'...");
await tcs.Task;
Console.WriteLine("Graceful shutdown.");

// Stop the provider.
provider.Stop();
Console.WriteLine("Provider stopped.");

// Stop the ctrlX Data Layer system.
system.Stop();
Console.WriteLine("ctrlX Data Layer system stopped.");