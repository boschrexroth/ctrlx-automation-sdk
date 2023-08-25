/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using comm.datalayer;
using Datalayer;
using Google.FlatBuffers;
using System;
using System.Collections.Concurrent;
using System.Linq;
using System.Threading;

namespace Samples.Datalayer.Provider.Virtual
{
    /// <summary>
    /// 
    /// </summary>
    internal class VirtualNodeHandler : IProviderNodeHandler
    {
        // If we know how many items we want to insert into the ConcurrentDictionary,
        // set the initial capacity to some prime number above that, to ensure that
        // the ConcurrentDictionary does not need to be resized while initializing it.       
        private const int InitialCapacity = 101;

        // The higher the concurrencyLevel, the higher the theoretical number of operations
        // that could be performed concurrently on the ConcurrentDictionary.  However, global
        // operations like resizing the dictionary take longer as the concurrencyLevel rises.
        // For the purposes of this example, we'll compromise at numCores * 2.
        private static readonly int ConcurrencyLevel = Environment.ProcessorCount * 2;

        //Fields

        /// <summary>
        /// Represents virtual light-weight Node
        /// </summary>
        private record VirtualNode(string Address, IVariant Value, NodeClass Category, bool IsBrowsable, bool IsReadable, bool IsWritable, bool IsCreatable, bool IsDeletable);

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
        /// Gets the ctrlX Data Layer provider
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
        public VirtualNodeHandler(IProvider provider, string baseAddress, string name)
        {
            Provider = provider;
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
            //We just listen to our base address using a wildcard on '{FullAddress}/**'
            var (result, _) = Provider.CreateNode(FullAddress, "**", this);
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            var folderNode = new VirtualNode(FullAddress, Variant.Null, NodeClass.Folder, true, false, false, false, false);
            _nodes.AddOrUpdate(folderNode.Address, folderNode, (k, v) => folderNode);

            //Create some virtual nodes here just for demonstration
            Console.WriteLine($"Creating virtual nodes on address: {FullAddress}");
            int currentLevel = 0;
            if (CreateDummyNodes(FullAddress, 3, ref currentLevel, 5).IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }

            // Start the Provider
            if (Provider.Start().IsBad())
            {
                Console.WriteLine("ctrlX Data Layer provider could not be started!");
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
            // Stop ctrlX Data Layer provider
            var result = Provider.Stop();
            Console.Write(value: $"Provider stopped: {result}");


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

            CreateMetadata(virtualNode, result);
        }

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
                var virtualNode = new VirtualNode(address, new Variant(_random.Next()), NodeClass.Variable, true, true, false, false, false);
                _nodes.AddOrUpdate(address, virtualNode, (k, v) => virtualNode);

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

        /// <summary>
        /// Creates meta informations for a variable
        /// </summary>
        /// <param name="node"></param>
        /// <param name="result"></param>
        private static void CreateMetadata(VirtualNode node, IProviderNodeResult result)
        {
            //Create MetaData
            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);

            var operations = AllowedOperations.CreateAllowedOperations(builder,
                read: node.IsReadable,
                write: node.IsWritable,
                browse: node.IsBrowsable,
                create: node.IsCreatable,
                delete: node.IsDeletable);

            var metaData = Metadata.CreateMetadata(builder,
                nodeClass: node.Category,
                operationsOffset: operations,
                descriptionOffset: builder.CreateString(node.Address),
                descriptionUrlOffset: builder.CreateString(node.Address));

            builder.Finish(metaData.Value);
            result.SetResult(DLR_RESULT.DL_OK, new Variant(builder));
        }

        #endregion
    }
}
