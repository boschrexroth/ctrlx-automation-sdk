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
using MQTTnet.Protocol;
using Samples.Datalayer.MQTT.Base;
using Samples.Datalayer.MQTT.Client;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Samples.Datalayer.MQTT.Pub
{
    /// <summary>
    /// Handler for a Pub configuration node
    /// </summary>
    internal class MqttPubConfigNodeHandler : MqttBaseNodeHandler
    {
        //Consts    


        //Fields

        //All Pub configs are using a same pool of subscriptions
        private static readonly Dictionary<uint, ISubscription> _subscriptionsByPublishInterval = new();

        /// <summary>
        /// Creates a handler
        /// </summary>
        /// <param name="root"></param>
        /// <param name="parent"></param>
        /// <param name="name"></param>
        public MqttPubConfigNodeHandler(MqttRootNodeHandler root, MqttBaseNodeHandler parent, string name) :
            base(root, parent, name)
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

            //Folder (self)
            var (result, node) = Root.Provider.CreateBranchNode(this, BaseAddress, Name, true);
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            //...

            //topic
            (result, node) = Root.Provider.CreateVariableNode(this, FullAddress, Names.Topic, new Variant(MqttClientWrapper.DefaultTopic));
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            //source-address
            //framework/metrics/system/memused-percent
            //framework/metrics/system/memused-mb
            //framework/metrics/system/cpu-utilisation-percent
            (result, node) = Root.Provider.CreateVariableNode(this, FullAddress, Names.SourceAddress, new Variant("framework/metrics/system/cpu-utilisation-percent"));
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            //publish-interval-millis
            (result, node) = Root.Provider.CreateVariableNode(this, FullAddress, Names.PublishIntervalMillis, new Variant(1_000));
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            //quality-of-service
            (result, node) = Root.Provider.CreateVariableNode(this, FullAddress, Names.QualityOfService, new Variant((int)MqttQualityOfServiceLevel.AtLeastOnce));
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            //message-expiry-interval-millis
            (result, node) = Root.Provider.CreateVariableNode(this, FullAddress, Names.MessageExpiryIntervalMillis, new Variant(TimeSpan.FromMinutes(60).TotalMilliseconds));
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            //retain
            (result, node) = Root.Provider.CreateVariableNode(this, FullAddress, Names.Retain, Variant.False);
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            //Subscribe to Data layer
            if (SubscribeDatalayer().IsBad())
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
            //Unsubscribe from Datalayer
            var result = UnsubscribeDatalayer();
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }

            return base.Stop();
        }

        /// <summary>
        /// OnWrite handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="writeValue"></param>
        /// <param name="result"></param>
        public override void OnWrite(string address, IVariant writeValue, IProviderNodeResult result)
        {
            //Fetch the node
            if (!Nodes.TryGetValue(address, out ProviderNodeWrapper wrappedNode))
            {
                result.SetResult(DLR_RESULT.DL_FAILED);
                return;
            }

            //Check for read-only nodes
            if (wrappedNode.IsReadOnly)
            {
                result.SetResult(DLR_RESULT.DL_FAILED);
                return;
            }

            //We're only interested in changed values
            var trimmedWriteValue = writeValue.Trim();
            if (wrappedNode.Value == trimmedWriteValue)
            {
                result.SetResult(DLR_RESULT.DL_FAILED);
                return;
            }

            switch (wrappedNode.Name)
            {
                //topic
                case Names.Topic:
                    if (writeValue.IsEmptyOrWhiteSpace())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    wrappedNode.Value = trimmedWriteValue;
                    break;

                //source-address
                case Names.SourceAddress:
                    if (writeValue.IsEmptyOrWhiteSpace())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Unsubscribe from current source-address
                    if (UnsubscribeDatalayer().IsBad())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Apply changes
                    wrappedNode.Value = trimmedWriteValue;

                    //Subscribe to new source-address
                    if (SubscribeDatalayer().IsBad())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }
                    break;

                //publish-interval-millis
                case Names.PublishIntervalMillis:
                    if (!writeValue.IsNumber)
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Validate write value
                    if (writeValue.ToInt32() <= 0)
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Unsubscribe from datalayer
                    if (UnsubscribeDatalayer().IsBad())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Apply changes
                    wrappedNode.Value = writeValue;

                    //Subscribe to datalayer
                    if (SubscribeDatalayer().IsBad())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }
                    break;

                //quality-of-service
                case Names.QualityOfService:
                    if (!writeValue.IsNumber)
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Valid QoS?
                    if (!MqttClientWrapper.IsValidQualityOfServiceLevel(writeValue.ToInt32()))
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    wrappedNode.Value = writeValue;
                    break;

                //retain
                case Names.Retain:
                    if (!writeValue.IsBool)
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    wrappedNode.Value = writeValue;
                    break;

                //message-expiry-interval-millis
                case Names.MessageExpiryIntervalMillis:
                    if (!writeValue.IsNumber)
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Validate write value
                    if (writeValue.ToInt32() < 0)
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    wrappedNode.Value = writeValue;
                    break;
            }

            //Success
            result.SetResult(DLR_RESULT.DL_OK);
        }

        /// <summary>
        /// OnRemove handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public override void OnRemove(string address, IProviderNodeResult result)
        {
            //Stop the handler
            if (Stop().IsBad())
            {
                result.SetResult(DLR_RESULT.DL_FAILED);
                return;
            }

            //Remove from parent
            Parent.Handlers.Remove(this);
            result.SetResult(DLR_RESULT.DL_OK);
        }

        #endregion

        #region Private

        /// <summary>
        /// Gets or creates a Data Layer subscription by current publish interval
        /// </summary>
        /// <returns></returns>
        private (DLR_RESULT, ISubscription) GetOrCreateDatalayerSubscriptionByPublishInterval()
        {
            var publishInterval = GetNode(Names.PublishIntervalMillis).Value.ToUInt32();

            //Find a subscription by matching publish interval
            if (_subscriptionsByPublishInterval.TryGetValue(publishInterval, out var subscription))
            {
                return (DLR_RESULT.DL_OK, subscription);
            }

            // Create new subscription
            var id = $"{FullAddress}_{publishInterval}".Replace("/", "_");
            Console.WriteLine($"Create Subscription (Id: {id}, PublishInterval: {publishInterval})");

            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
            var properties = SubscriptionProperties.CreateSubscriptionProperties(
                builder: builder,
                idOffset: builder.CreateString(id),
                keepaliveInterval: TimeoutMillis,
                publishInterval: publishInterval,
                rulesOffset: default,
                errorInterval: TimeoutMillis);
            builder.Finish(properties.Value);
            var propertiesFlatbuffers = new Variant(builder);

            var (result, newSubscription) = Root.Client.CreateSubscription(propertiesFlatbuffers, Variant.Null);
            if (result.IsBad())
            {
                return (result, null);
            }

            _subscriptionsByPublishInterval.Add(publishInterval, newSubscription);
            return (result, newSubscription);
        }

        /// <summary>
        /// Subscribes the given address of the datalayer
        /// </summary>
        /// <returns></returns>
        private DLR_RESULT SubscribeDatalayer()
        {
            var address = GetNode(Names.SourceAddress).Value.ToString();
            Console.WriteLine($"Subscribe Data Layer: {address}");

            //Request matching subscription
            var (result, subscription) = GetOrCreateDatalayerSubscriptionByPublishInterval();
            if (result.IsBad())
            {
                return result;
            }

            //Add DataChanged event handler
            subscription.DataChanged += OnDataChanged;

            //Subscribe the source-address

            return subscription.Subscribe(address);
        }


        /// <summary>
        /// Unsubscribes the given address from the Data Layer
        /// </summary>
        /// <returns></returns>
        private DLR_RESULT UnsubscribeDatalayer()
        {
            var address = GetNode(Names.SourceAddress).Value.ToString();
            Console.WriteLine($"Unsubscribe Data Layer: {address}");

            //Request matching Subscription
            var (result, subscription) = GetOrCreateDatalayerSubscriptionByPublishInterval();
            if (result.IsBad())
            {
                return result;
            }

            //Remove DataChanged event handler
            subscription.DataChanged -= OnDataChanged;

            //Check if we can unsubscribe
            if (IsSubscriptionInUse(address))
            {
                Console.WriteLine($"Unsubscribe skipped, because '{address}' is subscribed by another configuration.");
                return DLR_RESULT.DL_OK;
            }

            //Unsubscribe from source-address
            return subscription.Unsubscribe(address);
        }

        /// <summary>
        /// Indicates whether the given address can be unsubscribed from Data Layer
        /// </summary>
        /// <param name="address"></param>
        /// <returns></returns>
        private bool IsSubscriptionInUse(string address)
        {
            //We have to check if our source address is not used by another Pub handler
            return Parent.Handlers.Exists(handler => handler != this && handler.HasNode(Names.SourceAddress) && handler.GetNode(Names.SourceAddress).Value.ToString() == address);
        }

        /// <summary>
        /// Data Layer subscription data changed event
        /// </summary>
        /// <param name="subscription"></param>
        /// <param name="args"></param>
        private async void OnDataChanged(ISubscription subscription, IDataChangedEventArgs args)
        {
            //Check if node has been already removed
            if (IsShutdown)
            {
                return;
            }

            //Extract notification information
            var notifyInfo = NotifyInfo.GetRootAsNotifyInfo(args.Item.Info.ToFlatbuffers());

            //We're only interested in good values
            if (args.Result.IsBad())
            {
                return;
            }

            //Publish to MQTT without blocking the event queue
            var task = PublishMqttAsync(notifyInfo.Node, args.Item.Value);
            await task;
        }

        /// <summary>
        /// Publishes the value of a Data Layer address to current topic
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        private async Task<DLR_RESULT> PublishMqttAsync(string address, IVariant value)
        {
            var topic = GetNode(Names.Topic).Value.ToString();
            var json = value.ToJson();

            Console.WriteLine($"Publish '{address}' -> Topic '{topic}' -> {json} ...");

            var task = Root.MqttClient.PublishAsync(
                topic: topic,
                payload: json,
                quality: (MqttQualityOfServiceLevel)GetNode(Names.QualityOfService).Value.ToInt32(),
                messageExpiryIntervalMillis: GetNode(Names.MessageExpiryIntervalMillis).Value.ToUInt32(),
                retain: GetNode(Names.Retain).Value.ToBool());

            var result = await task;
            return result;
        }

        #endregion
    }
}
