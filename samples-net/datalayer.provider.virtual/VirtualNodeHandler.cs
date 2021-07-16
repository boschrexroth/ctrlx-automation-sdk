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

using comm.datalayer;
using Datalayer;
using FlatBuffers;
using System;
using System.Collections.Concurrent;
using System.Linq;
using System.Net;
using System.Threading;

namespace Samples.Datalayer.Provider.Virtual
{
    /// <summary>
    /// 
    /// </summary>
    internal class VirtualNodeHandler : IProviderNodeHandler
    {
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!! CHANGE THIS TO YOUR ENVIRONMENT !!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        private static readonly IPAddress IpAddress = IPAddress.Parse("192.168.1.250");
        private static readonly string Username = "boschrexroth";
        private static readonly string Password = "boschrexroth";

        // If we know how many items we want to insert into the ConcurrentDictionary,
        // set the initial capacity to some prime number above that, to ensure that
        // the ConcurrentDictionary does not need to be resized while initializing it.       
        private static readonly int InitialCapacity = 101;

        // The higher the concurrencyLevel, the higher the theoretical number of operations
        // that could be performed concurrently on the ConcurrentDictionary.  However, global
        // operations like resizing the dictionary take longer as the concurrencyLevel rises.
        // For the purposes of this example, we'll compromise at numCores * 2.
        private static readonly int ConcurrencyLevel = Environment.ProcessorCount * 2;

        //Fields

        /// <summary>
        /// Represents virtual light-weight Node
        /// </summary>
        private record VirtualNode(string Address, IVariant Value, bool IsReadOnly);

        /// <summary>
        /// The map we're holding our nodes by address
        /// </summary>
        private readonly ConcurrentDictionary<string, VirtualNode> _nodes = new(ConcurrencyLevel, InitialCapacity);

        /// <summary>
        /// Random is used to create demo nodes
        /// </summary>
        private readonly Random _random = new();

        #region Properties

        /// <summary>
        /// Gets the application lock
        /// </summary>
        public ManualResetEvent Lock { get; } = new(false);

        /// <summary>
        /// Gets the DatalayerSystem
        /// </summary>
        public DatalayerSystem System { get; } = new();

        /// <summary>
        /// Gets the Data Layer provider
        /// </summary>
        public IProvider Provider { get; private set; }

        /// <summary>
        /// Gets the name
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// Gets the base address
        /// </summary>
        public string BaseAddress { get; }

        /// <summary>
        /// Gets the full address with is BaseAddress/Name
        /// </summary>
        public string FullAddress { get; }

        #endregion

        /// <summary>
        /// Creates a handler
        /// </summary>
        /// <param name="baseAddress"></param>
        /// <param name="name"></param>
        public VirtualNodeHandler(string baseAddress, string name)
        {
            BaseAddress = baseAddress ?? throw new ArgumentNullException(nameof(baseAddress));
            Name = name ?? throw new ArgumentNullException(nameof(name));

            FullAddress = $"{BaseAddress}/{Name}";
        }

        #region Public

        /// <summary>
        /// Starts the handler
        /// </summary>
        /// <returns></returns>
        public DLR_RESULT Start()
        {
            // Check if the process is running inside a snap 
            var isSnapped = IsSnapped;
            Console.WriteLine($"Running inside snap: {isSnapped}");

            //Starts the Data Layer system without a new broker (startBroker = false), because one broker is already running on ctrlX device
            System.Start(startBroker: false);

            if (!System.IsStarted)
            {
                Console.WriteLine("Data Layer could not be started!");
                return DLR_RESULT.DL_FAILED;
            }

            //Create a provider with inter-process communication (ipc) protocol if running in snap, otherwise tcp
            Provider = isSnapped
                ? System.Factory.CreateIpcProvider()
                : System.Factory.CreateTcpProvider(IpAddress,
                    DatalayerSystem.DefaultProviderPort,
                    Username,
                    Password);

            //We just listen to our base address using a wildcard on '{FullAddress}/**'
            var (result, _) = Provider.CreateNode(FullAddress, "**", this);
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }

            //Create some virtual nodes here just for demonstration
            int currentLevel = 0;
            if (CreateDummyNodes(FullAddress, 3, ref currentLevel, 5).IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }

            // Start the Provider
            if (Provider.Start().IsBad())
            {
                Console.WriteLine("Data Layer provider could not be started!");
                return DLR_RESULT.DL_FAILED;
            }

            return DLR_RESULT.DL_OK;
        }

        /// <summary>
        /// Stops the handler
        /// </summary>
        /// <returns></returns>
        public DLR_RESULT Stop()
        {
            // Stop Data Layer provider
            var result = Provider.Stop();
            Console.Write(value: $"Provider stopped: {result}");

            // Stop the Data Layer system
            System.Stop();
            Console.WriteLine("Data Layer system stopped");

            // Clear our nodes
            _nodes.Clear();

            //Unlock application
            Lock.Set();

            return DLR_RESULT.DL_OK;
        }

        #endregion

        #region Public Static

        /// <summary>
        /// Indicates whether the application is snapped or not (running inside linux snappy environment)
        /// </summary>
        /// <returns></returns>
        public static bool IsSnapped => !string.IsNullOrEmpty(Environment.GetEnvironmentVariable("SNAP"));

        #endregion

        #region Private

        /// <summary>
        /// Creates some virtual dummy nodes
        /// </summary>
        /// <param name="currentPath"></param>
        /// <param name="nodesPerLevel"></param>
        /// <param name="currentLevel"></param>
        /// <param name="maxDepth"></param>
        /// <returns></returns>
        private DLR_RESULT CreateDummyNodes(string currentPath, int nodesPerLevel, ref int currentLevel, int maxDepth)
        {
            const int NameLength = 5;

            currentLevel++;

            for (int i = 0; i < nodesPerLevel; i++)
            {
                var address = $"{currentPath}/{RandomString(NameLength)}";

                //Add the nodes to our map by address
                var virtualNode = new VirtualNode(address, new Variant(_random.Next()), true);
                _nodes.AddOrUpdate(address, virtualNode, (k, v) => virtualNode);

                Console.WriteLine($"Created a virtual node on address: {address}");

                if (currentLevel < maxDepth)
                {
                    CreateDummyNodes(address, nodesPerLevel, ref currentLevel, maxDepth);
                }
            }

            currentLevel--;
            return DLR_RESULT.DL_OK;
        }

        /// <summary>
        /// Returns a random string
        /// </summary>
        /// <param name="length"></param>
        /// <returns></returns>
        private string RandomString(int length)
        {
            const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            return new string(Enumerable.Repeat(chars, length).Select(s => s[_random.Next(s.Length)]).ToArray());
        }

        #endregion

        #region Event Handler

        /// <summary>
        /// OnCreate handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="args"></param>
        /// <param name="result"></param>

        public void OnCreate(string address, IVariant args, IProviderNodeResult result)
        {
            Console.WriteLine($"OnCreate {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// OnRemove handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public void OnRemove(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnRemove {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// OnBrowse handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public void OnBrowse(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnBrowse {address}");

            //We return the list of level names of the current branch, not leaf nodes which hold a value itself.

            //Split given address into parts
            var addressParts = address.Split('/');

            var branchLevelNames = _nodes
                //Exclude leaf nodes and filter for current level nodes
                .Where(pair => pair.Key.Length > address.Length && pair.Key.StartsWith(address + '/'))
                //Extract current branch level name
                .Select(n =>
                {
                    var parts = n.Key.Split('/');
                    return parts.Length > addressParts.Length ? parts[addressParts.Length] : parts[^1];

                }).ToArray();

            result.SetResult(DLR_RESULT.DL_OK, new Variant(branchLevelNames));
        }

        /// <summary>
        /// OnRead handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="args"></param>
        /// <param name="result"></param>
        public void OnRead(string address, IVariant args, IProviderNodeResult result)
        {
            Console.WriteLine($"OnRead {address}: {args}");

            //Fetch the node by address
            if (!_nodes.TryGetValue(address, out var virtualNode))
            {
                result.SetResult(DLR_RESULT.DL_FAILED);
                return;
            }

            result.SetResult(DLR_RESULT.DL_OK, virtualNode.Value);
        }

        /// <summary>
        /// OnWrite handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="writeValue"></param>
        /// <param name="result"></param>
        public void OnWrite(string address, IVariant writeValue, IProviderNodeResult result)
        {
            Console.WriteLine($"OnWrite {address}: {writeValue}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// OnMetadata handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public void OnMetadata(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnMetadata {address}");

            //Fetch the node by address
            if (!_nodes.TryGetValue(address, out var virtualNode))
            {
                result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
                return;
            }

            //Create MetaData
            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);

            var operations = AllowedOperations.CreateAllowedOperations(builder,
                create: false, //Default
                delete: false, //Default
                read: true, //Default
                write: !virtualNode.IsReadOnly); //Default

            var metaData = Metadata.CreateMetadata(builder,
                operationsOffset: operations,
                descriptionOffset: builder.CreateString(""),
                descriptionUrlOffset: builder.CreateString("")
            );

            builder.Finish(metaData.Value);
            result.SetResult(DLR_RESULT.DL_OK, new Variant(builder));
        }

        #endregion
    }
}
