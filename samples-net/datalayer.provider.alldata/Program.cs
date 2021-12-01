/*
MIT License

Copyright (c) 2021 Bosch Rexroth AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

using Datalayer;

namespace Samples.Datalayer.Provider.Alldata
{
    using System;
    using System.Diagnostics;
    using System.IO;
    using System.Net;
    using System.Threading;

    /// <summary>
    /// Defines the <see cref="Program" />.
    /// </summary>
    internal class Program
    {
        /// <summary>
        /// Defines the IpAddress.
        /// </summary>
        private static readonly IPAddress IpAddress = IPAddress.Parse("192.168.1.1");// IPAddress.Loopback

        /// <summary>
        /// Defines the Username.
        /// </summary>
        private static readonly string Username = "boschrexroth";

        /// <summary>
        /// Defines the Password.
        /// </summary>
        private static readonly string Password = "boschrexroth";

        /// <summary>
        /// The Main.
        /// </summary>
        /// <param name="args">The args<see cref="string[]"/>.</param>
        internal static void Main(string[] args)
        {
            //Add app exit handler to handle optional clean up
            AppDomain.CurrentDomain.ProcessExit += CurrentDomain_ProcessExit;

            // Check if the process is running inside a snap 
            var isSnapped = !string.IsNullOrEmpty(Environment.GetEnvironmentVariable("SNAP"));
            Console.WriteLine($"Running inside snap: {isSnapped}");

            // Create a new ctrlX Data Layer system
            using var system = new DatalayerSystem();

            // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
            system.Start();
            Console.WriteLine("ctrlX Data Layer system started.");

            // Create the provider with inter-process communication (ipc) protocol if running in snap, otherwise tcp
            using var provider = isSnapped
                ? system.Factory.CreateIpcProvider()
                : system.Factory.CreateTcpProvider(IpAddress,
                    DatalayerSystem.DefaultProviderPort,
                    Username,
                    Password);
            Console.WriteLine("ctrlX Data Layer provider created.");

            // Register type with binary flatbuffers schema file: sampleSchema.bfbs (auto generated from sampleSchema.fbs by flatc compiler)
            var resultRegisterType = provider.RegisterType(DataTypes.InertialValue.Address, Path.Combine(AppContext.BaseDirectory, "sampleSchema.bfbs"));
            Console.WriteLine($"Registering Type with address='{DataTypes.InertialValue.Address}', result='{resultRegisterType}'");


            // Create the static nodes
            var staticNodes = new[]
            {
                Factory.CreateStaticNode( new Variant(false), DataTypes.Bool8),
                Factory.CreateStaticNode( new Variant(sbyte.MaxValue), DataTypes.Int8),
                Factory.CreateStaticNode( new Variant(byte.MaxValue), DataTypes.Uint8),
                Factory.CreateStaticNode(new Variant(short.MaxValue), DataTypes.Int16),
                Factory.CreateStaticNode( new Variant(ushort.MaxValue), DataTypes.Uint16),
                Factory.CreateStaticNode( new Variant(int.MaxValue), DataTypes.Int32),
                Factory.CreateStaticNode( new Variant(uint.MaxValue), DataTypes.Uint32),
                Factory.CreateStaticNode( new Variant(long.MaxValue), DataTypes.Int64),
                Factory.CreateStaticNode( new Variant(ulong.MaxValue), DataTypes.Uint64),
                Factory.CreateStaticNode( new Variant(float.MaxValue), DataTypes.Float32),
                Factory.CreateStaticNode( new Variant(double.MaxValue), DataTypes.Float64),
                Factory.CreateStaticNode( new Variant("String_0"), DataTypes.String),
                Factory.CreateStaticNode( Factory.CreateInertialValue(30, -442, 911), DataTypes.InertialValue),
                Factory.CreateStaticNode(new Variant(new bool[] {true, false, true}), DataTypes.ArrayOfBool8),
                Factory.CreateStaticNode(new Variant(new sbyte[] { sbyte.MinValue, -1, 0, sbyte.MaxValue }), DataTypes.ArrayOfInt8),
                Factory.CreateStaticNode(new Variant(new byte[] {byte.MinValue, byte.MaxValue}), DataTypes.ArrayOfUint8),
                Factory.CreateStaticNode(new Variant(new short[] {short.MinValue, -1, 0, short.MaxValue}), DataTypes.ArrayOfInt16),
                Factory.CreateStaticNode(new Variant(new ushort[]{ushort.MinValue, ushort.MaxValue}), DataTypes.ArrayOfUint16),
                Factory.CreateStaticNode(new Variant(new int[] {int.MinValue, -1, 0, int.MaxValue}), DataTypes.ArrayOfInt32),
                Factory.CreateStaticNode(new Variant(new uint[] {uint.MinValue, uint.MaxValue}), DataTypes.ArrayOfUint32),
                Factory.CreateStaticNode(new Variant(new long[]{ long.MinValue, -1, 0, long.MaxValue }), DataTypes.ArrayOfInt64),
                Factory.CreateStaticNode(new Variant(new ulong[]{ ulong.MinValue, ulong.MaxValue }), DataTypes.ArrayOfUint64),
                Factory.CreateStaticNode(new Variant(new float[] {float.MinValue, -1.0f, 0.0f, float.MaxValue}), DataTypes.ArrayOfFloat32),
                Factory.CreateStaticNode(new Variant(new double[]{ double.MinValue, -1.0, 0.0, double.MaxValue }), DataTypes.ArrayOfFloat64),
                Factory.CreateStaticNode(new Variant(new string[] { "Blue", "Red", "Orange", "Yellow" }), DataTypes.ArrayOfString)
            };

            // Register all static nodes with ReadOnlyNodeHandler
            foreach (var node in staticNodes)
            {
                var handler = new ReadWriteNodeHandler(node);
                provider.RegisterNode(node.Address, handler);
            }

            // Create the dynamic nodes
            var dynamicNodes = new[]
            {
                Factory.CreateDynamicNode(new Variant(false), DataTypes.Bool8),
                Factory.CreateDynamicNode(new Variant(sbyte.MinValue), DataTypes.Int8),
                Factory.CreateDynamicNode(new Variant(byte.MinValue), DataTypes.Uint8),
                Factory.CreateDynamicNode(new Variant(short.MinValue), DataTypes.Int16),
                Factory.CreateDynamicNode(new Variant( ushort.MinValue), DataTypes.Uint16),
                Factory.CreateDynamicNode(new Variant( int.MinValue), DataTypes.Int32),
                Factory.CreateDynamicNode(new Variant(uint.MinValue), DataTypes.Uint32),
                Factory.CreateDynamicNode(new Variant(long.MinValue), DataTypes.Int64),
                Factory.CreateDynamicNode(new Variant(ulong.MinValue), DataTypes.Uint64),
                Factory.CreateDynamicNode(new Variant(0f), DataTypes.Float32),
                Factory.CreateDynamicNode(new Variant(0.0), DataTypes.Float64),
                Factory.CreateDynamicNode(new Variant("String_0"), DataTypes.String),
                Factory.CreateDynamicNode(Factory.CreateInertialValue(0,0,0), DataTypes.InertialValue),
                Factory.CreateDynamicNode(new Variant(new bool[] {true, false, true, false, true, false, true, false, true, false }), DataTypes.ArrayOfBool8),
                Factory.CreateDynamicNode(new Variant(new sbyte[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfInt8),
                Factory.CreateDynamicNode(new Variant(new byte[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint8),
                Factory.CreateDynamicNode(new Variant(new short[] {-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }), DataTypes.ArrayOfInt16),
                Factory.CreateDynamicNode(new Variant(new ushort[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint16),
                Factory.CreateDynamicNode(new Variant(new int[] {-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }), DataTypes.ArrayOfInt32),
                Factory.CreateDynamicNode(new Variant(new uint[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint32),
                Factory.CreateDynamicNode(new Variant(new long[]{-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }), DataTypes.ArrayOfInt64),
                Factory.CreateDynamicNode(new Variant(new ulong[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), DataTypes.ArrayOfUint64),
                Factory.CreateDynamicNode(new Variant(new float[] {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f }), DataTypes.ArrayOfFloat32),
                Factory.CreateDynamicNode(new Variant(new double[] {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 }), DataTypes.ArrayOfFloat64),
                Factory.CreateDynamicNode(new Variant(new string[] { "String_0", "String_1", "String_2", "String_3", "String_4", "String_5", "String_6", "String_7", "String_8", "String_9" }), DataTypes.ArrayOfString)
            };

            // Register all dynamic nodes with ReadWriteNodeHandler
            foreach (var node in dynamicNodes)
            {
                var handler = new ReadOnlyNodeHandler(node);
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
                if (provider.IsConnected)
                {

                    var sw = Stopwatch.StartNew();
                    sw.Start();
                    // Change dynamic nodes value every 1000 milliseconds.
                    foreach (var node in dynamicNodes)
                    {
                        node.ChangeValue();
                    }
                    sw.Stop();
                    Console.WriteLine($"Change value elapsed ms: {sw.Elapsed.TotalMilliseconds}");
                }
                else
                {
                    Console.WriteLine("Provider is disconnected: skip changing values of dynamic nodes.");
                }

                Thread.Sleep(1000);
            }
        }

        /// <summary>
        /// The CurrentDomain_ProcessExit.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private static void CurrentDomain_ProcessExit(object sender, EventArgs e)
        {
            Console.WriteLine("Application exit");
        }
    }
}
