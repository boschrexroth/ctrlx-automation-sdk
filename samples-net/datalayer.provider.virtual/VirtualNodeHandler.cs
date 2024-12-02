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

namespace Samples.Datalayer.Provider.Virtual
{
    /// <summary>
    /// The VirtualNodeHandler handles the virtual node instances.
    /// </summary>
    /// <param name="rootAddress"></param>
    /// <param name="name"></param>
    internal class VirtualNodeHandler(IProvider provider, string rootAddress) : IProviderNodeHandler
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
        /// Represents virtual light-weight Node.
        /// </summary>
        private record VirtualNode(string Address, IVariant Value, NodeClass Category, bool IsBrowsable, bool IsReadable, bool IsWritable, bool IsCreatable, bool IsDeletable);

        /// <summary>
        /// The map we're holding our nodes by address.
        /// </summary>
        private readonly ConcurrentDictionary<string, VirtualNode> _nodes = new(ConcurrencyLevel, InitialCapacity);

        /// <summary>
        /// Random is used to create demo nodes.
        /// </summary>
        private readonly Random _random = new();

        #region Properties

        /// <summary>
        /// Gets the ctrlX Data Layer provider.
        /// </summary>
        public IProvider Provider { get; private set; } = provider;

        /// <summary>
        /// Gets the root address.
        /// </summary>
        public string RootAddress { get; } = rootAddress;

        #endregion

        #region Public

        /// <summary>
        /// Starts the handler.
        /// </summary>
        /// <returns></returns>
        public DLR_RESULT Start()
        {
            // We just listen to our root address using a wildcard on '{RootAddress}/**'.
            var (result, _) = Provider.CreateNode(RootAddress, "**", this);
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            var rootNode = new VirtualNode(RootAddress, Variant.Null, NodeClass.Folder, true, false, false, true, false);
            _nodes.AddOrUpdate(rootNode.Address, rootNode, (k, v) => rootNode);

            // Create some virtual nodes here just for demonstration.
            Console.WriteLine($"Creating virtual nodes on root address: {RootAddress}");
            int currentLevel = 0;
            if (CreateDummyNodes(RootAddress, 3, ref currentLevel, 5).IsBad())
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
        /// Stops the handler.
        /// </summary>
        /// <returns></returns>
        public DLR_RESULT Stop()
        {
            // Stop ctrlX Data Layer provider.
            var result = Provider.Stop();
            Console.Write(value: $"Provider stopped: {result}");

            // Clear our nodes.
            _nodes.Clear();

            return DLR_RESULT.DL_OK;
        }

        #endregion

        #region Event Handler

        /// <summary>
        /// OnCreate handler.
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
        /// OnRemove handler.
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public void OnRemove(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnRemove {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// OnBrowse handler.
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public void OnBrowse(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnBrowse {address}");

            // We return the list of level names of the current branch, not leaf nodes which hold a value itself.

            // Split given address into parts.
            var addressParts = address.Split('/');

            var branchLevelNames = _nodes
                // Exclude leaf nodes and filter for current level nodes.
                .Where(pair => pair.Key.Length > address.Length && pair.Key.StartsWith(address + '/'))
                // Extract current branch level name.
                .Select(n =>
                {
                    var parts = n.Key.Split('/');
                    return parts.Length > addressParts.Length ? parts[addressParts.Length] : parts[^1];

                }).ToArray();

            result.SetResult(DLR_RESULT.DL_OK, new Variant(branchLevelNames));
        }

        /// <summary>
        /// OnRead handler.
        /// </summary>
        /// <param name="address"></param>
        /// <param name="args"></param>
        /// <param name="result"></param>
        public void OnRead(string address, IVariant args, IProviderNodeResult result)
        {
            Console.WriteLine($"OnRead {address}: {args}");

            // Fetch the node by address.
            if (!_nodes.TryGetValue(address, out var node))
            {
                result.SetResult(DLR_RESULT.DL_FAILED);
                return;
            }

            result.SetResult(DLR_RESULT.DL_OK, node.Value);
        }

        /// <summary>
        /// OnWrite handler.
        /// </summary>
        /// <param name="address"></param>
        /// <param name="writeValue"></param>
        /// <param name="result"></param>
        public void OnWrite(string address, IVariant writeValue, IProviderNodeResult result)
        {
            Console.WriteLine($"OnWrite {address}: {writeValue}");

            // Fetch the node by address.
            if (!_nodes.TryGetValue(address, out var node))
            {
                result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
                return;
            }

            // Because records are immutable, we have to replace the node.
            var newNode = new VirtualNode(node.Address, writeValue, node.Category, node.IsBrowsable, node.IsReadable, node.IsWritable, node.IsCreatable, node.IsDeletable);
            _nodes.AddOrUpdate(address, node, (k, v) => newNode);
            result.SetResult(DLR_RESULT.DL_OK, newNode.Value);
        }

        /// <summary>
        /// OnMetadata handler.
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public void OnMetadata(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnMetadata {address}");

            // Fetch the node by address.
            if (!_nodes.TryGetValue(address, out var node))
            {
                result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
                return;
            }

            CreateMetadata(node, result);
        }

        #endregion

        #region Private

        /// <summary>
        /// Creates some virtual dummy nodes.
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

            // We add some nodes (int32) to our map by address.
            for (int i = 0; i < nodesPerLevel; i++)
            {
                var address = $"{currentPath}/{RandomString(NameLength)}";
                var node = new VirtualNode(address, new Variant(_random.Next()), NodeClass.Variable, true, true, true, false, false);
                _nodes.AddOrUpdate(address, node, (k, v) => node);

                if (currentLevel < maxDepth)
                {
                    CreateDummyNodes(address, nodesPerLevel, ref currentLevel, maxDepth);
                }
            }

            currentLevel--;
            return DLR_RESULT.DL_OK;
        }

        /// <summary>
        /// Returns a random string.
        /// </summary>
        /// <param name="length"></param>
        /// <returns></returns>
        private string RandomString(int length)
        {
            const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            return new string(Enumerable.Repeat(chars, length).Select(s => s[_random.Next(s.Length)]).ToArray());
        }

        /// <summary>
        /// Creates meta informations for a variable.
        /// </summary>
        /// <param name="node"></param>
        /// <param name="result"></param>
        private void CreateMetadata(VirtualNode node, IProviderNodeResult result)
        {
            // Metadata for the root node (here: folder).
            if (node.Address == RootAddress)
            {
                var rootMdb = new MetadataBuilder(
                    AllowedOperationFlags.Read |
                    AllowedOperationFlags.Browse,
                    node.Address);

                rootMdb.SetNodeClass(NodeClass.Folder);
                result.SetResult(DLR_RESULT.DL_OK, rootMdb.Build());
                return;
            }

            // Metadata for child nodes (int32 variables).
            var mdb = new MetadataBuilder(
                AllowedOperationFlags.Read |
                AllowedOperationFlags.Write |
                AllowedOperationFlags.Browse,
                node.Address);

            mdb.SetNodeClass(NodeClass.Variable);
            mdb.AddReference(ReferenceType.ReadType, "types/datalayer/int32");
            mdb.AddReference(ReferenceType.WriteInType, "types/datalayer/int32");
            result.SetResult(DLR_RESULT.DL_OK, mdb.Build());
        }

        #endregion
    }
}