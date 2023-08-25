/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;

namespace Samples.Datalayer.Provider.Alldata
{
    using comm.datalayer;
    using Google.FlatBuffers;
    using sample.schema;
    using System;
    using System.IO;
    using System.Threading;

    /// <summary>
    /// Defines the <see cref="Program" />.
    /// </summary>
    internal class Program
    {
        // Please define node folder names in ctrlX Data Layer
        private const string ROOT = "samples/dotnet/all-data";
        private const string STATIC = "static";
        private const string DYNAMIC = "dynamic";

        /// <summary>
        /// The Main method is the entry point of an executable app.
        /// </summary>
        /// <param name="args">The args<see cref="string[]"/>.</param>
        internal static void Main(string[] args)
        {
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
            var resultRegisterType = provider.RegisterType(DataTypes.InertialValue.Address, Path.Combine(AppContext.BaseDirectory, "sampleSchema.bfbs"));
            Console.WriteLine($"Registering Type with address='{DataTypes.InertialValue.Address}', result='{resultRegisterType}'");

            var utcNow = DateTime.UtcNow;

            // Create the static nodes
            var staticNodes = new[]
            {
               CreateStaticNode(new Variant(false), DataTypes.Bool8),
               CreateStaticNode(new Variant(sbyte.MaxValue), DataTypes.Int8),
               CreateStaticNode(new Variant(byte.MaxValue), DataTypes.Uint8),
               CreateStaticNode(new Variant(short.MaxValue), DataTypes.Int16),
               CreateStaticNode(new Variant(ushort.MaxValue), DataTypes.Uint16),
               CreateStaticNode(new Variant(int.MaxValue), DataTypes.Int32),
               CreateStaticNode(new Variant(uint.MaxValue), DataTypes.Uint32),
               CreateStaticNode(new Variant(long.MaxValue), DataTypes.Int64),
               CreateStaticNode(new Variant(ulong.MaxValue), DataTypes.Uint64),
               CreateStaticNode(new Variant(float.MaxValue), DataTypes.Float32),
               CreateStaticNode(new Variant(double.MaxValue), DataTypes.Float64),
               CreateStaticNode(new Variant("String_0"), DataTypes.String),
               CreateStaticNode(new Variant(utcNow), DataTypes.Timestamp),
               CreateStaticNode(CreateInertialValue(30, -442, 911), DataTypes.InertialValue),

               CreateStaticNode(new Variant(new bool[] {true, false, true}), DataTypes.ArrayOfBool8),
               CreateStaticNode(new Variant(new sbyte[] { sbyte.MinValue, -1, 0, sbyte.MaxValue }), DataTypes.ArrayOfInt8),
               CreateStaticNode(new Variant(new byte[] {byte.MinValue, byte.MaxValue}), DataTypes.ArrayOfUint8),
               CreateStaticNode(new Variant(new short[] {short.MinValue, -1, 0, short.MaxValue}), DataTypes.ArrayOfInt16),
               CreateStaticNode(new Variant(new ushort[]{ushort.MinValue, ushort.MaxValue}), DataTypes.ArrayOfUint16),
               CreateStaticNode(new Variant(new int[] {int.MinValue, -1, 0, int.MaxValue}), DataTypes.ArrayOfInt32),
               CreateStaticNode(new Variant(new uint[] {uint.MinValue, uint.MaxValue}), DataTypes.ArrayOfUint32),
               CreateStaticNode(new Variant(new long[]{ long.MinValue, -1, 0, long.MaxValue }), DataTypes.ArrayOfInt64),
               CreateStaticNode(new Variant(new ulong[]{ ulong.MinValue, ulong.MaxValue }), DataTypes.ArrayOfUint64),
               CreateStaticNode(new Variant(new float[] {float.MinValue, -1.0f, 0.0f, float.MaxValue}), DataTypes.ArrayOfFloat32),
               CreateStaticNode(new Variant(new double[]{ double.MinValue, -1.0, 0.0, double.MaxValue }), DataTypes.ArrayOfFloat64),
               CreateStaticNode(new Variant(new string[] { "Blue", "Red", "Orange", "Yellow" }), DataTypes.ArrayOfString),
               CreateStaticNode(new Variant(new DateTime[] { utcNow, DateTime.Now, DateTime.Today }), DataTypes.ArrayOfTimestamp)
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
                CreateDynamicNode(new Variant(false), DataTypes.Bool8),
                CreateDynamicNode(new Variant(sbyte.MinValue), DataTypes.Int8),
                CreateDynamicNode(new Variant(byte.MinValue), DataTypes.Uint8),
                CreateDynamicNode(new Variant(short.MinValue), DataTypes.Int16),
                CreateDynamicNode(new Variant( ushort.MinValue), DataTypes.Uint16),
                CreateDynamicNode(new Variant( int.MinValue), DataTypes.Int32),
                CreateDynamicNode(new Variant(uint.MinValue), DataTypes.Uint32),
                CreateDynamicNode(new Variant(long.MinValue), DataTypes.Int64),
                CreateDynamicNode(new Variant(ulong.MinValue), DataTypes.Uint64),
                CreateDynamicNode(new Variant(0f), DataTypes.Float32),
                CreateDynamicNode(new Variant(0.0), DataTypes.Float64),
                CreateDynamicNode(new Variant("String_0"), DataTypes.String),
                CreateDynamicNode(new Variant(utcNow), DataTypes.Timestamp),
                CreateDynamicNode(CreateInertialValue(0,0,0), DataTypes.InertialValue),
                CreateDynamicNode(new Variant(new bool[] {true, false, true, false, true, false, true, false, true, false }), DataTypes.ArrayOfBool8),
                CreateDynamicNode(new Variant(new sbyte[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfInt8),
                CreateDynamicNode(new Variant(new byte[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint8),
                CreateDynamicNode(new Variant(new short[] {-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }), DataTypes.ArrayOfInt16),
                CreateDynamicNode(new Variant(new ushort[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint16),
                CreateDynamicNode(new Variant(new int[] {-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }), DataTypes.ArrayOfInt32),
                CreateDynamicNode(new Variant(new uint[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint32),
                CreateDynamicNode(new Variant(new long[]{-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }), DataTypes.ArrayOfInt64),
                CreateDynamicNode(new Variant(new ulong[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint64),
                CreateDynamicNode(new Variant(new float[] {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f }), DataTypes.ArrayOfFloat32),
                CreateDynamicNode(new Variant(new double[] {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 }), DataTypes.ArrayOfFloat64),
                CreateDynamicNode(new Variant(new string[] { "String_0", "String_1", "String_2", "String_3", "String_4", "String_5", "String_6", "String_7", "String_8", "String_9" }), DataTypes.ArrayOfString),
                CreateDynamicNode(new Variant(new DateTime[] { utcNow, utcNow, utcNow, utcNow, utcNow, utcNow, utcNow, utcNow }), DataTypes.ArrayOfTimestamp)
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

            // Check if provider is connected.
            Console.WriteLine($"Provider connected: {provider.IsConnected}");
            if (!provider.IsConnected)
            {
                //if not we exit and retry after app daemon restart-delay (see snapcraft.yaml)
                Console.WriteLine($"Restarting app after restart-delay of 10 s ...");
                return;
            }

            // Check if provider is started.
            Console.WriteLine($"Provider started: {startResult}");

            // Just keep the process running
            while (true)
            {
                if (!provider.IsConnected)
                {
                    Console.WriteLine("Provider is disconnected: skip changing values of dynamic nodes.");
                    continue;
                }

                // Change dynamic nodes value every 1000 milliseconds.
                foreach (var node in dynamicNodes)
                {
                    node.ChangeValue(DateTime.UtcNow);
                }

                Thread.Sleep(1000);
            }
        }


        /// <summary>
        /// Creates a static node.
        /// </summary>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <param name="dataType">The dataType<see cref="DataType"/>.</param>
        /// <returns>The <see cref="IProviderNode"/>.</returns>
        private static Node CreateStaticNode(IVariant value, DataType dataType)
        {
            var address = $"{ROOT}/{STATIC}/{dataType.Name}";
            var description = $"{STATIC} data with type {dataType.Name}";

            var metaData = new MetadataBuilder(AllowedOperationFlags.Read | AllowedOperationFlags.Write, description)
                .SetNodeClass(NodeClass.Variable)
                .AddReference(ReferenceType.ReadType, dataType.Address)
                .AddReference(ReferenceType.WriteType, dataType.Address)
                .Build();

            return new Node(address, value, metaData);
        }

        /// <summary>
        /// Creates a dynamic node.
        /// </summary>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <param name="dataType">The dataType<see cref="DataType"/>.</param>
        private static Node CreateDynamicNode(IVariant value, DataType dataType)
        {
            var address = $"{ROOT}/{DYNAMIC}/{dataType.Name}";
            var description = $"{DYNAMIC} data with type {dataType.Name}";

            var metaData = new MetadataBuilder(AllowedOperationFlags.Read, description)
                  .SetNodeClass(NodeClass.Variable)
                  .AddReference(ReferenceType.ReadType, dataType.Address)
                  .Build();

            return new Node(address, value, metaData);
        }

        /// <summary>
        /// Creates an InertialValue flatbuffers.
        /// </summary>
        /// <param name="x">The x<see cref="short"/>.</param>
        /// <param name="y">The y<see cref="short"/>.</param>
        /// <param name="z">The z<see cref="short"/>.</param>
        /// <returns>The <see cref="Variant"/>.</returns>
        private static Variant CreateInertialValue(short x, short y, short z)
        {
            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
            var offset = InertialValue.CreateInertialValue(builder, x, y, z);
            builder.Finish(offset.Value);
            return new Variant(builder);
        }
    }
}
