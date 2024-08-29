/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;
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

// Check if client is connected
if (!client.IsConnected)
{
    // Initially exit and retry after app restart-delay (see snapcraft.yaml)
    Console.WriteLine($"Client is not connected -> exit");
    return;
}

var addresses = new[]
{
    "framework/metrics/system/cpu-utilisation-percent",
    "framework/metrics/system/memavailable-mb",
    "framework/metrics/system/membuffers-mb",
    "framework/metrics/system/memcache-mb",
    "framework/metrics/system/memfree-mb",
    "framework/metrics/system/memtotal-mb",
    "framework/metrics/system/memused-percent"
};

Console.WriteLine("BulkRead");
var (result, items) = client.BulkRead(addresses);
if (result.IsGood())
{
    foreach (var item in items)
    {
        Console.WriteLine($"address: {item.Address}, value: {item.Value.ToFloat()}, timestamp: {item.Timestamp}, result: {item.Result}");
    }
}

//We're using the async bulk read method just for demonstration here.
//Hint: If you're not using any async calls, you can remove async keyword from the Main() method signature.
Console.WriteLine("BulkReadAsync");
var task = client.BulkReadAsync(addresses);
var taskResult = await task;

if (taskResult.Result.IsGood())
{
    foreach (var item in items)
    {
        Console.WriteLine($"address: {item.Address}, value: {item.Value.ToFloat()}, timestamp: {item.Timestamp}, result: {item.Result}");
    }
}

// Wait for process termination
Console.WriteLine("Waiting for process exit event 'SIGTERM'...");
await tcs.Task;
Console.WriteLine("Graceful shutdown app");

// Stop the ctrlX Data Layer system
system.Stop();
Console.WriteLine("ctrlX Data Layer system stopped.");