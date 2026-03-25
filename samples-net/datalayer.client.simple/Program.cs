/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using comm.datalayer;
using Datalayer;
using System;
using System.Threading.Tasks;

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

// Create a Datalayer Client instance and connect. Automatically reconnects if the connection is interrupted.
using var client = system.Factory.CreateClient(remote);
Console.WriteLine("ctrlX Data Layer client created.");

// Check if client is connected.
if (!client.IsConnected)
{
    // Initially exit and retry after app restart-delay (see snapcraft.yaml).
    Console.WriteLine($"Client is not connected -> exit");
    return;
}

// Read a single value.
var address = "framework/metrics/system/cpu-utilisation-percent";
var (result, value) = client.Read(address);
if (result.IsBad())
{
    Console.WriteLine($"{result}");
    return;
}
Console.WriteLine($"{DateTime.UtcNow}, {address}: {value.ToFloat()} (Read)");

// Subscribe for a value change.

// Configure the subscription by using generated Flatbuffers type SubscriptionPropertiesT.
var props = new SubscriptionPropertiesT()
{
    Id = "mySubscription",
    PublishInterval = 1000,
    KeepaliveInterval = 10000,
    ErrorInterval = 10000,
};
var (createResult, subscription) = client.CreateSubscription(props, userData: null);

// =========================================================================================================
// Alternative: Configure the subscription properties by using Flatbuffers helper SubscriptionPropertiesBuilder.
// =========================================================================================================
//
// var propertiesFlatbuffers = new SubscriptionPropertiesBuilder("mySubscription")
//     .SetKeepAliveIntervalMillis(10000)
//     .SetPublishIntervalMillis(1000)
//     .SetErrorIntervalMillis(10000)
//     .Build();
//
// var (createResult, subscription) = client.CreateSubscription(propertiesFlatbuffers, userData: null);

// =========================================================================================================
// Alternative: Configure the subscription properties by using Flatbuffers SubscriptionProperties.
// ========================================================================================================= 
//
// var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
// var properties = SubscriptionProperties.CreateSubscriptionProperties(
//    builder: builder,
//    idOffset: builder.CreateString("mySubscription"),
//    keepaliveInterval: 10000,
//    publishInterval: 1000,
//    errorInterval: 10000);
// builder.Finish(properties.Value);
//
// var propertiesFlatbuffers = new Variant(builder);
// var (createResult, subscription) = client.CreateSubscription(propertiesFlatbuffers, userData: null);
// =========================================================================================================

if (createResult.IsBad())
{
    Console.WriteLine($"Failed to create subscription: {createResult}");
    return;
}

// Add DataChanged Event Handler.
subscription.DataChanged += async (_, args) =>
{
    var notifyInfo = NotifyInfo.GetRootAsNotifyInfo(args.Item.Info.ToFlatbuffers());
    var timestampUtc = DateTime.FromFileTimeUtc(Convert.ToInt64(notifyInfo.Timestamp));
    var value = args.Item.Value.ToFloat();
    Console.WriteLine($"{timestampUtc}, {notifyInfo.Node}: {value} (Subscribe)");

    // Calling synchronous Datalayer API methods in a subscription callback context is not allowed (WOULD_BLOCK).
    // Instead we have to use an async event handler signature in combination of corresponding Async* method to call.

    // We randomly read async here just for demonstration.
    if (value - Convert.ToInt32(value) < 0.5)
    {
        var readResult = await client.ReadAsync(address);
        if (readResult.Result.IsBad())
        {
            Console.WriteLine($"{readResult.Result}");
        }
        else
        {
            Console.WriteLine($"{DateTime.UtcNow}, {address}: {readResult.Value.ToFloat()} (ReadAsync)");
        }
    }
};

// Subscribe to the value of an address.
var subscribeResult = subscription.Subscribe(address);
if (subscribeResult.IsBad())
{
    Console.WriteLine($"Failed to subscribe: {subscribeResult}");
    return;
}

// Wait for process termination.
Console.WriteLine("Waiting for process exit event 'SIGTERM'...");
await tcs.Task;
Console.WriteLine("Graceful shutdown.");

// Stop the ctrlX Data Layer system.
system.Stop();
Console.WriteLine("ctrlX Data Layer system stopped.");