/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using System;
using System.IO;
using System.Threading.Tasks;
using Datalayer;
using Google.FlatBuffers;
using sample.schema;
using Samples.Datalayer.Provider;

// Please define the node root folder in ctrlX Data Layer
const string root = "sdk/net/provider/simple";

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

// Create the provider with remote connection string
using var provider = system.Factory.CreateProvider(remote);
Console.WriteLine("ctrlX Data Layer provider created.");

// Create and register node on given address and read-only callbacks.
var myIntNode = Node.CreateReadOnlyNode($"{root}/myInt", "types/datalayer/int32", "MyInt Description", new Variant(42));
var (resultMyInt, _) = provider.RegisterNode(myIntNode.Address, new ReadOnlyNodeHandler(myIntNode));
Console.WriteLine($"Registering Node with address='{myIntNode.Address}', result='{resultMyInt}'");

// Create and register node on given address (read-only).
var myDoubleNode = Node.CreateReadOnlyNode($"{root}/myDouble", "types/datalayer/float64", "MyDouble Description", new Variant(Math.PI));
var (resultMyDouble, _) = provider.RegisterNode(myDoubleNode.Address, new ReadOnlyNodeHandler(myDoubleNode));
Console.WriteLine($"Registering Node with address='{myDoubleNode.Address}', result='{resultMyDouble}'");

// Create and register node on given address (read/write).
var myStringNode = Node.CreateReadWriteNode($"{root}/myString", "types/datalayer/string", "MyString Description", new Variant("Hello ctrlX"));
var (resultMyString, _) = provider.RegisterNode(myStringNode.Address, new ReadWriteNodeHandler(myStringNode));
Console.WriteLine($"Registering Node with address='{myStringNode.Address}', result='{resultMyString}'");

// Register type with binary flatbuffers schema file: sampleSchema.bfbs (auto generated from sampleSchema.fbs by flatc compiler)
const string typeAddressInertialValue = $"types/{root}/inertial-value";
var resultRegisterType = provider.RegisterType(typeAddressInertialValue, Path.Combine(AppContext.BaseDirectory, "bfbs", "sampleSchema.bfbs"));
Console.WriteLine($"Registering Type with address='{typeAddressInertialValue}', result='{resultRegisterType}'");

// Create flatbuffers value with FlatbufferBuilder (flatbuffers c# API) and InertialValue (auto generated from sampleSchema.fbs by flatc compiler) 
var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
var offset = InertialValue.CreateInertialValue(builder, 30, -442, 911);
builder.Finish(offset.Value);
var variantFlatbuffers = new Variant(builder);

// Create and register node on given address and read-only callbacks.
var myFlatbuffersNode = Node.CreateReadOnlyNode($"{root}/inertial-value", typeAddressInertialValue, "My Inertial Value Description", variantFlatbuffers);
var (resultMyFlatbuffers, _) = provider.RegisterNode(myFlatbuffersNode.Address, new ReadOnlyNodeHandler(myFlatbuffersNode));
Console.WriteLine($"Registering Node with address='{myFlatbuffersNode.Address}', result='{resultMyFlatbuffers}'");

// Start the Provider
var startResult = provider.Start();

// Check if provider is started.
Console.WriteLine(value: $"Provider started: {startResult}");

// Check if provider is connected
if (!provider.IsConnected)
{
    // We exit and retry after app daemon restart-delay (see snapcraft.yaml)
    Console.WriteLine($"Provider is not connected -> exit");
    return;
}


// Wait for process termination
Console.WriteLine("Waiting for process exit event 'SIGTERM'...");
await tcs.Task;
Console.WriteLine("Graceful shutdown app");

// Stop the provider
provider.Stop();
Console.WriteLine("Provider stopped.");

// Stop the ctrlX Data Layer system
system.Stop();
Console.WriteLine("ctrlX Data Layer system stopped.");