/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using System;
using System.IO;
using System.Threading.Tasks;
using Datalayer;
using Samples.Datalayer.Provider.Alldata;

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
system.Start();
Console.WriteLine("ctrlX Data Layer system started.");

// Create a remote address with the parameters according to your environment
var remote = new Remote(ip: "192.168.1.1", sslPort: 443).ToString();

// Create the provider with remote connection string
using var provider = system.Factory.CreateProvider(remote);

// Register type with binary flatbuffers schema file: sampleSchema.bfbs (auto generated from sampleSchema.fbs by flatc compiler)
var resultRegisterType = provider.RegisterType(DataTypes.InertialValue.Address, Path.Combine(AppContext.BaseDirectory, "bfbs", "sampleSchema.bfbs"));
Console.WriteLine($"Registering Type with address='{DataTypes.InertialValue.Address}', result='{resultRegisterType}'");

var utcNow = DateTime.UtcNow;

// Create the static nodes
var staticNodes = new[]
{
    Node.CreateStatic(new Variant(false), DataTypes.Bool8),
    Node.CreateStatic(new Variant(sbyte.MaxValue), DataTypes.Int8),
    Node.CreateStatic(new Variant(byte.MaxValue), DataTypes.Uint8),
    Node.CreateStatic(new Variant(short.MaxValue), DataTypes.Int16),
    Node.CreateStatic(new Variant(ushort.MaxValue), DataTypes.Uint16),
    Node.CreateStatic(new Variant(int.MaxValue), DataTypes.Int32),
    Node.CreateStatic(new Variant(uint.MaxValue), DataTypes.Uint32),
    Node.CreateStatic(new Variant(long.MaxValue), DataTypes.Int64),
    Node.CreateStatic(new Variant(ulong.MaxValue), DataTypes.Uint64),
    Node.CreateStatic(new Variant(float.MaxValue), DataTypes.Float32),
    Node.CreateStatic(new Variant(double.MaxValue), DataTypes.Float64),
    Node.CreateStatic(new Variant("String_0"), DataTypes.String),
    Node.CreateStatic(new Variant(utcNow), DataTypes.Timestamp),
    Node.CreateStatic(Node.CreateInertialValue(30, -442, 911), DataTypes.InertialValue),

    Node.CreateStatic(new Variant(new bool[] {true, false, true}), DataTypes.ArrayOfBool8),
    Node.CreateStatic(new Variant(new sbyte[] { sbyte.MinValue, -1, 0, sbyte.MaxValue }), DataTypes.ArrayOfInt8),
    Node.CreateStatic(new Variant(new byte[] {byte.MinValue, byte.MaxValue}), DataTypes.ArrayOfUint8),
    Node.CreateStatic(new Variant(new short[] {short.MinValue, -1, 0, short.MaxValue}), DataTypes.ArrayOfInt16),
    Node.CreateStatic(new Variant(new ushort[]{ushort.MinValue, ushort.MaxValue}), DataTypes.ArrayOfUint16),
    Node.CreateStatic(new Variant(new int[] {int.MinValue, -1, 0, int.MaxValue}), DataTypes.ArrayOfInt32),
    Node.CreateStatic(new Variant(new uint[] {uint.MinValue, uint.MaxValue}), DataTypes.ArrayOfUint32),
    Node.CreateStatic(new Variant(new long[]{ long.MinValue, -1, 0, long.MaxValue }), DataTypes.ArrayOfInt64),
    Node.CreateStatic(new Variant(new ulong[]{ ulong.MinValue, ulong.MaxValue }), DataTypes.ArrayOfUint64),
    Node.CreateStatic(new Variant(new float[] {float.MinValue, -1.0f, 0.0f, float.MaxValue}), DataTypes.ArrayOfFloat32),
    Node.CreateStatic(new Variant(new double[]{ double.MinValue, -1.0, 0.0, double.MaxValue }), DataTypes.ArrayOfFloat64),
    Node.CreateStatic(new Variant(new string[] { "Blue", "Red", "Orange", "Yellow" }), DataTypes.ArrayOfString),
    Node.CreateStatic(new Variant(new DateTime[] { utcNow, DateTime.Now, DateTime.Today }), DataTypes.ArrayOfTimestamp)
};

// Register all static nodes (read/write) using the ReadWriteNodeHandler.
// Change to ReadOnlyNodeHandler for read-only.
foreach (var node in staticNodes)
{
    var handler = new ReadWriteNodeHandler(node);
    provider.RegisterNode(node.Address, handler);
}

// Create the dynamic nodes
var dynamicNodes = new[]
{
    Node.CreateDynamic(new Variant(false), DataTypes.Bool8),
    Node.CreateDynamic(new Variant(sbyte.MinValue), DataTypes.Int8),
    Node.CreateDynamic(new Variant(byte.MinValue), DataTypes.Uint8),
    Node.CreateDynamic(new Variant(short.MinValue), DataTypes.Int16),
    Node.CreateDynamic(new Variant( ushort.MinValue), DataTypes.Uint16),
    Node.CreateDynamic(new Variant( int.MinValue), DataTypes.Int32),
    Node.CreateDynamic(new Variant(uint.MinValue), DataTypes.Uint32),
    Node.CreateDynamic(new Variant(long.MinValue), DataTypes.Int64),
    Node.CreateDynamic(new Variant(ulong.MinValue), DataTypes.Uint64),
    Node.CreateDynamic(new Variant(0f), DataTypes.Float32),
    Node.CreateDynamic(new Variant(0.0), DataTypes.Float64),
    Node.CreateDynamic(new Variant("String_0"), DataTypes.String),
    Node.CreateDynamic(new Variant(utcNow), DataTypes.Timestamp),
    Node.CreateDynamic(Node.CreateInertialValue(0,0,0), DataTypes.InertialValue),
    Node.CreateDynamic(new Variant(new bool[] {true, false, true, false, true, false, true, false, true, false }), DataTypes.ArrayOfBool8),
    Node.CreateDynamic(new Variant(new sbyte[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfInt8),
    Node.CreateDynamic(new Variant(new byte[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint8),
    Node.CreateDynamic(new Variant(new short[] {-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }), DataTypes.ArrayOfInt16),
    Node.CreateDynamic(new Variant(new ushort[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint16),
    Node.CreateDynamic(new Variant(new int[] {-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }), DataTypes.ArrayOfInt32),
    Node.CreateDynamic(new Variant(new uint[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint32),
    Node.CreateDynamic(new Variant(new long[]{-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }), DataTypes.ArrayOfInt64),
    Node.CreateDynamic(new Variant(new ulong[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint64),
    Node.CreateDynamic(new Variant(new float[] {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f }), DataTypes.ArrayOfFloat32),
    Node.CreateDynamic(new Variant(new double[] {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 }), DataTypes.ArrayOfFloat64),
    Node.CreateDynamic(new Variant(new string[] { "String_0", "String_1", "String_2", "String_3", "String_4", "String_5", "String_6", "String_7", "String_8", "String_9" }), DataTypes.ArrayOfString),
    Node.CreateDynamic(new Variant(new DateTime[] { utcNow, utcNow, utcNow, utcNow, utcNow, utcNow, utcNow, utcNow }), DataTypes.ArrayOfTimestamp)
};

// Register all dynamic nodes (read/write) using the ReadWriteNodeHandler.
// Change to ReadOnlyNodeHandler for read-only.
foreach (var node in dynamicNodes)
{
    var handler = new ReadWriteNodeHandler(node);
    provider.RegisterNode(node.Address, handler);
}

// Start the Provider
var startResult = provider.Start();

// Check if provider is started.
Console.WriteLine($"Provider started: {startResult}");

// Check if provider is connected
if (!provider.IsConnected)
{
    // We exit and retry after app daemon restart-delay (see snapcraft.yaml)
    Console.WriteLine($"Provider is not connected -> exit");
    return;
}


_ = Task.Run(async () =>
{
    while (true)
    {
        // Change dynamic nodes value every 1000 milliseconds.
        foreach (var node in dynamicNodes)
        {
            node.ChangeValue(DateTime.UtcNow);
        }

        await Task.Delay(1000);
    }
});

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