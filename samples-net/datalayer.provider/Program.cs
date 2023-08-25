/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;

namespace Samples.Datalayer.Provider
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
        // Please define the node root folder in ctrlX Data Layer
        private const string ROOT = "samples/dotnet";

        /// <summary>
        /// The Main method is the entry point of an executable app.
        /// </summary>
        /// <param name="args">The args<see cref="string[]"/>.</param>
        internal static void Main(string[] args)
        {
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
            var myIntNode = CreateReadOnlyNode($"{ROOT}/myInt", "types/datalayer/int32", "MyInt Description", new Variant(42));
            var (resultMyInt, _) = provider.RegisterNode(myIntNode.Address, new ReadOnlyNodeHandler(myIntNode));
            Console.WriteLine($"Registering Node with address='{myIntNode.Address}', result='{resultMyInt}'");

            // Create and register node on given address (read-only).
            var myDoubleNode = CreateReadOnlyNode($"{ROOT}/myDouble", "types/datalayer/float64", "MyDouble Description", new Variant(Math.PI));
            var (resultMyDouble, _) = provider.RegisterNode(myDoubleNode.Address, new ReadOnlyNodeHandler(myDoubleNode));
            Console.WriteLine($"Registering Node with address='{myDoubleNode.Address}', result='{resultMyDouble}'");

            // Create and register node on given address (read/write).
            var myStringNode = CreateReadWriteNode($"{ROOT}/myString", "types/datalayer/string", "MyDouble Description", new Variant("Hello ctrlX"));
            var (resultMyString, _) = provider.RegisterNode(myStringNode.Address, new ReadWriteNodeHandler(myStringNode));
            Console.WriteLine($"Registering Node with address='{myStringNode.Address}', result='{resultMyString}'");

            // Register type with binary flatbuffers schema file: sampleSchema.bfbs (auto generated from sampleSchema.fbs by flatc compiler)
            const string typeAddressInertialValue = "types/sample/schema/inertial-value";
            var resultRegisterType = provider.RegisterType(typeAddressInertialValue, Path.Combine(AppContext.BaseDirectory, "sampleSchema.bfbs"));
            Console.WriteLine($"Registering Type with address='{typeAddressInertialValue}', result='{resultRegisterType}'");

            // Create flatbuffers value with FlatbufferBuilder (flatbuffers c# API) and InertialValue (auto generated from sampleSchema.fbs by flatc compiler) 
            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
            var offset = InertialValue.CreateInertialValue(builder, 30, -442, 911);
            builder.Finish(offset.Value);
            var variantFlatbuffers = new Variant(builder);

            // Create and register node on given address and read-only callbacks.
            var myFlatbuffersNode = CreateReadOnlyNode($"{ROOT}/inertial-value", typeAddressInertialValue, "My Inertial Value Description", variantFlatbuffers);
            var (resultMyFlatbuffers, _) = provider.RegisterNode(myFlatbuffersNode.Address, new ReadOnlyNodeHandler(myFlatbuffersNode));
            Console.WriteLine($"Registering Node with address='{myFlatbuffersNode.Address}', result='{resultMyFlatbuffers}'");

            // Start the Provider
            var startResult = provider.Start();

            // Check if provider is connected.
            Console.WriteLine(value: $"Provider connected: {provider.IsConnected}");

            if (!provider.IsConnected)
            {
                //if not we exit and retry after app daemon restart-delay (see snapcraft.yaml)
                Console.WriteLine($"Restarting app after restart-delay of 10 s ...");
                return;
            }

            // Check if provider is started.
            Console.WriteLine(value: $"Provider started: {startResult}");

            // Just keep the process running
            while (true)
            {
                if (!provider.IsConnected)
                {
                    Console.WriteLine("Provider is disconnected: skip changing values of dynamic nodes.");
                }

                Thread.Sleep(millisecondsTimeout: 1000);
            }
        }

        /// <summary>
        /// The CreateNode.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="targetAddress">The targetAddress<see cref="string"/>.</param>
        /// <param name="description">The description<see cref="string"/>.</param>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <returns>The <see cref="Node"/>.</returns>
        public static Node CreateReadOnlyNode(string address, string targetAddress, string description, IVariant value)
        {
            var metaData = new MetadataBuilder(AllowedOperationFlags.Read, description)
                .SetNodeClass(NodeClass.Variable)
                .AddReference(ReferenceType.ReadType, targetAddress)
                .Build();

            return new Node(address, value, metaData);
        }

        /// <summary>
        /// The CreateReadWriteNode.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="targetAddress">The targetAddress<see cref="string"/>.</param>
        /// <param name="description">The description<see cref="string"/>.</param>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <returns>The <see cref="Node"/>.</returns>
        public static Node CreateReadWriteNode(string address, string targetAddress, string description, IVariant value)
        {
            var metaData = new MetadataBuilder(AllowedOperationFlags.Read | AllowedOperationFlags.Write, description)
                .SetNodeClass(NodeClass.Variable)
                .AddReference(ReferenceType.ReadType, targetAddress)
                .AddReference(ReferenceType.WriteType, targetAddress)
                .Build();

            return new Node(address, value, metaData);
        }
    }
}
