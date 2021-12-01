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
using MQTTnet;
using MQTTnet.Protocol;
using Samples.Datalayer.MQTT.Base;
using System;
using System.Collections.Concurrent;
using System.Linq;
using System.Text;

namespace Samples.Datalayer.MQTT.Sub
{
    /// <summary>
    /// Handler for the scanner node
    /// </summary>
    internal class MqttSubScannerNodeHandler : MqttSubBaseNodeHandler
    {
        // If we know how many items we want to insert into the ConcurrentDictionary,
        // set the initial capacity to some prime number above that, to ensure that
        // the ConcurrentDictionary does not need to be resized while initializing it.       
        private static readonly int InitialCapacity = 101;

        // The higher the concurrencyLevel, the higher the theoretical number of operations
        // that could be performed concurrently on the ConcurrentDictionary.  However, global
        // operations like resizing the dictionary take longer as the concurrencyLevel rises.
        // For the purposes of this example, we'll compromise at numCores * 2.
        private static readonly int ConcurrencyLevel = Environment.ProcessorCount * 2;
        /// <summary>
        /// Represents a virtual light-weight MQTT Node
        /// </summary>
        //private record MqttNode(string Address, IVariant Value);

        //Fields
        private readonly ConcurrentDictionary<string, IVariant> _nodes = new(ConcurrencyLevel, InitialCapacity);

        /// <summary>
        /// Creates the handler
        /// </summary>
        /// <param name="root"></param>
        /// <param name="parent"></param>
        public MqttSubScannerNodeHandler(MqttRootNodeHandler root, MqttBaseNodeHandler parent) :
            base(root, parent, "scanner")
        {
        }

        #region Overrides

        /// <summary>
        /// Starts the handler
        /// </summary>
        /// <returns></returns>
        public override DLR_RESULT Start()
        {
            //Create, register and add the handled nodes here

            //We listen to our base address using a wildcard on '{FullAddress}/**' which is 'scanner/**'
            var (result, _) = Root.Provider.CreateBranchNode(this, FullAddress, "**");
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }

            return base.Start();
        }

        /// <summary>
        /// Stops the handler
        /// </summary>
        /// <returns></returns>
        public override DLR_RESULT Stop()
        {
            //Clear our nodes
            _nodes.Clear();

            return base.Stop();
        }

        /// <summary>
        /// Returns the Topic
        /// </summary>
        protected override string Topic => "#";

        /// <summary>
        /// Returns the Quality
        /// </summary>
        protected override MqttQualityOfServiceLevel Quality => MqttQualityOfServiceLevel.AtLeastOnce;

        /// <summary>
        /// OnRead handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="args"></param>
        /// <param name="result"></param>
        public override void OnRead(string address, IVariant args, IProviderNodeResult result)
        {
            //Fetch the node by address

            //Branches -> No value
            if (!_nodes.TryGetValue(address, out var value))
            {
                result.SetResult(DLR_RESULT.DL_FAILED);
                return;
            }

            //Leaf -> Return value
            result.SetResult(DLR_RESULT.DL_OK, value);
        }

        /// <summary>
        /// OnMetadata handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public override void OnMetadata(string address, IProviderNodeResult result)
        {
            //'scanner' node (folder)
            if (address.Equals(FullAddress))
            {
                CreateFolderMetadata(address, result);
                return;
            }

            //Virtual nodes
            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
            var operations = AllowedOperations.CreateAllowedOperations(builder,
               read: true,
               write: false,
               browse: true, // We don't know the node's dynamic hierarchy
               create: false,
               delete: false);

            var metaData = Metadata.CreateMetadata(builder,
                nodeClass: NodeClass.Variable,
                operationsOffset: operations,
                descriptionOffset: builder.CreateString(address),
                descriptionUrlOffset: builder.CreateString(address));
            builder.Finish(metaData.Value);
            result.SetResult(DLR_RESULT.DL_OK, new Variant(builder));
        }

        /// <summary>
        /// Creates Folder Metadata
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        private static void CreateFolderMetadata(string address, IProviderNodeResult result)
        {
            //Create MetaData
            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);

            var folderOperations = AllowedOperations.CreateAllowedOperations(builder,
                   read: false,
                   write: false,
                   browse: true,
                   create: false,
                   delete: false);

            var folderMetaData = Metadata.CreateMetadata(builder,
                nodeClass: NodeClass.Folder,
                operationsOffset: folderOperations,
                descriptionOffset: builder.CreateString(address),
                descriptionUrlOffset: builder.CreateString(address));

            builder.Finish(folderMetaData.Value);
            result.SetResult(DLR_RESULT.DL_OK, new Variant(builder));
            return;
        }

        /// <summary>
        /// OnBrowse handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public override void OnBrowse(string address, IProviderNodeResult result)
        {
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
        /// MQTT message received event handler
        /// </summary>
        /// <param name="args"></param>
        protected override void OnMessageReceived(MqttApplicationMessageReceivedEventArgs args)
        {
            //We have to filter for our subscription to prevent duplicates if any other SUB is interested in same topic
            if (args.ApplicationMessage.SubscriptionIdentifiers[0] != SubscriptionId)
            {
                return;
            }

            //Filter for null topic which may occure
            if (args.ApplicationMessage.Topic == null)
            {
                return;
            }

            //Filter for null payload which may occure
            if (args.ApplicationMessage.Payload == null)
            {
                return;
            }

            //Stringify the payload
            var stringifiedPayload = Encoding.UTF8.GetString(args.ApplicationMessage.Payload);

            //Create a Variant of best matching type (Supported int32, double, string)
            var value = ToVariant(stringifiedPayload);
            var address = $"{FullAddress}/{args.ApplicationMessage.Topic}";
            //var node = new MqttNode(address, value);

            //Add node if not existing or update with current value
            _nodes.AddOrUpdate(address, value, (k, v) => value);
        }

        #endregion
    }
}
