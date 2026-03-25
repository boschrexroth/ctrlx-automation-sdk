/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
using comm.datalayer;
using Datalayer;
using System.Collections.Concurrent;
using System.Threading;

using TasK = System.Threading.Tasks.Task; // Provide collision with comm.datalayer.Task

namespace Samples.Datalayer.Provider
{
    using System;
    using System.Collections.Generic;

    /// <summary>
    /// SubscriptionNodeHandler provides nodes, publishing subscribed values on demand, instead of getting polled in sample interval.
    ///  
    /// This is recommended for providers having costly OnRead() implementations. 
    /// The provider has to implement the new Methods OnSubscribe(), OnUnsubscribe() and IProviderSubscription.Publish()
    /// to notify any changes for subscriptions, holding the nodes (adresses), which are subscribed by various clients.
    /// </summary>
    /// <remarks>
    /// Initializes a new instance of the <see cref="SubscriptionNodeHandler"/> class.
    /// </remarks>
    /// <param name="node">The node<see cref="Node"/>.</param>
    internal class SubscriptionNodeHandler(Node node) : ISubscriptionProviderNodeHandler
    {
        /// <summary>
        /// Gets the Node.
        /// </summary>
        public Node Node { get; private set; } = node;

        // Fields
        private readonly Random _rand = new();

        private readonly CancellationTokenSource _cts = new();

        private TasK _publisherTask;

        /// <summary>
        /// The subscriptions.
        /// </summary>
        private readonly ConcurrentDictionary<string, IProviderSubscription> _subscriptions = [];

        #region ISubscriptionProviderNodeHandler

        /// <summary>
        /// The OnCreate callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="args">The args<see cref="IVariant"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnCreate(string address, IVariant args, IProviderNodeResult result)
        {
            Console.WriteLine($"OnCreate {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// The OnRemove callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnRemove(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnRemove {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// The OnBrowse callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnBrowse(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnBrowse {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// The OnRead callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="args">The args<see cref="IVariant"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnRead(string address, IVariant args, IProviderNodeResult result)
        {
            Console.WriteLine($"OnRead {address}: {args}");
            result.SetResult(DLR_RESULT.DL_OK, Node.Value);
        }

        /// <summary>
        /// The OnWrite callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="writeValue">The writeValue<see cref="IVariant"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnWrite(string address, IVariant writeValue, IProviderNodeResult result)
        {
            Console.WriteLine($"OnWrite {address}: {writeValue}");
            Node.Value = writeValue;
            result.SetResult(DLR_RESULT.DL_OK, Node.Value);
        }

        /// <summary>
        /// The OnMetadata callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnMetadata(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnMetadata {address}");
            result.SetResult(DLR_RESULT.DL_OK, Node.Metadata);
        }

        /// <summary>
        /// OnSubscribe callback handler.
        /// </summary>
        /// <param name="address"></param>
        /// <param name="subscription"></param>
        public void OnSubscribe(string address, IProviderSubscription subscription)
        {
            // We have to manage all subscriptions, created by any clients.
            // This event handler is called, if a client calls Subscribe() on the given address (node) on his owned subscription.
            // Any client subscribed for one of our nodes: Add the new incoming subscription.
            var props = subscription.Properties;
            _subscriptions.TryAdd(props.Id, subscription);
        }

        /// <summary>
        /// OnUnsubscribe callback handler.
        /// </summary>
        /// <param name="address"></param>
        /// <param name="subscription"></param>
        public void OnUnsubscribe(string address, IProviderSubscription subscription)
        {
            // We have to manage all subscriptions, created by any clients.
            // This event handler is called, if a client calls UnSubscribe() on the given address (node) on his owned subscription.
            // Any client unsubscribed for one of our nodes: Remove the gone subscription.

            // Remove the subscription, if there are no items present anymore. 
            if (subscription.Nodes.Length == 0)
            {
                var props = subscription.Properties;
                _subscriptions.TryRemove(props.Id, out _);
            }
        }

        #endregion
        #region Change and Publish Management

        /// <summary>
        /// Starts publishing.
        /// </summary>
        public void StartPublish()
        {
            if (_publisherTask != null)
            {
                return;
            }

             Console.WriteLine($"Start publishing ...");
            _publisherTask = StartPublisherTask(_cts);
        }

        /// <summary>
        /// Stops publishing.
        /// </summary>
        public bool StopPublish()
        {
            if (_publisherTask == null)
            {
                return true;
            }
            Console.WriteLine($"Stop publishing ...");
            _cts.Cancel();
            var result = _publisherTask.Wait(10000);
            _publisherTask = null;
            return result;
        }

        /// <summary>
        /// Starts the publisher task.
        /// </summary>
        /// <param name="cts"></param>
        private TasK StartPublisherTask(CancellationTokenSource cts)
        {
            var task = TasK.Run(() =>
            {
                List<NotifyItem> items = [];

                while (true)
                {
                    if (cts.Token.IsCancellationRequested)
                    {
                        return;
                    }

                    // Different subscriptions can contain same addresses, so we have to update all subscriptions.
                    var now = DateTime.Now;
                    var timestamp = Convert.ToUInt64(now.ToFileTimeUtc()); // (uint64 (FILETIME) 64 bit 100ns since 1.1.1601 (UTC))
                    
                    foreach (var subscription in _subscriptions.Values)
                    {
                        items.Clear();
                        UpdateNodes(timestamp, subscription.Nodes, ref items);
                        subscription.Publish(DLR_RESULT.DL_OK, [.. items]);
                        Console.WriteLine($"{now} Publish: {Node.Address} ({Node.Value.ToInt32()})");
                    }

                    // We don't care of the per subscription configured publishing interval just for demonstration here.
                    Thread.Sleep(Convert.ToInt32(ISubscription.DefaultPublishIntervalMillis));
                }
            }, cts.Token);

            return task;
        }

        /// <summary>
        /// Updates some node(s) value as NotifyItem changes.
        /// </summary>
        /// <param name="timestamp"></param>
        /// <param name="nodes"></param>
        /// <returns></returns>
        private void UpdateNodes(ulong timestamp, string[] nodes, ref List<NotifyItem> items)
        {   
            for (var i = 0; i < nodes.Length; i++)
            {
                // Filter for our node.
                if (!nodes[i].Equals(Node.Address, StringComparison.OrdinalIgnoreCase))
                {
                    continue;
                }

                // Create a random value.
                Node.Value = new Variant(_rand.Next());

                // Create notification info.
                var info = new NotifyInfoT
                {
                    NotifyType = NotifyType.Data,       // This is a data change notification (value).
                    Node = nodes[i],                    // The address of the node.
                    SourceName = nodes[i].ToString(),   // The source of the notification, must not be same node.
                    EventType = Node.TypeAddress,       // The address of the node's type (e.g. 'types/datalayer/int32' or 'types/myflatbuffers').
                    //SequenceNumber = ...              // Needs to be increased for NotifyType.Event, only.
                    Timestamp = timestamp               // The timestamp of the notification.
                };

                // Add change with random value.
                items.Add(new NotifyItem(Node.Value, info));
            }
        }
        #endregion
    }
}