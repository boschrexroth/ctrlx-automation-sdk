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
using MQTTnet;
using MQTTnet.Protocol;
using MQTTnet.Server;
using Samples.Datalayer.MQTT.Base;
using Samples.Datalayer.MQTT.Client;
using System;
using System.Text;

namespace Samples.Datalayer.MQTT.Sub
{
    /// <summary>
    /// Handler for Sub configuration node
    /// </summary>
    internal class MqttSubConfigNodeHandler : MqttSubBaseNodeHandler
    {
        /// <summary>
        /// Creates the handler
        /// </summary>
        /// <param name="root"></param>
        /// <param name="parent"></param>
        /// <param name="name"></param>
        public MqttSubConfigNodeHandler(MqttRootNodeHandler root, MqttBaseNodeHandler parent, string name) :
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

            //target-address
            (result, node) = Root.Provider.CreateVariableNode(this, FullAddress, Names.TargetAddress, new Variant(Root.Test.Dummy1));
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            //json-data-type
            (result, node) = Root.Provider.CreateVariableNode(this, FullAddress, Names.JsonDataType, new Variant("float"));
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

            return base.Start();
        }

        /// <summary>
        /// Returns the Topic
        /// </summary>
        protected override string Topic => GetNode(Names.Topic).Value.ToString();

        /// <summary>
        /// Resturns the Quality
        /// </summary>
        protected override MqttQualityOfServiceLevel Quality => (MqttQualityOfServiceLevel)GetNode(Names.Topic).Value.ToInt32();

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

                    //Unsubscribe from current MQTT topic (sync)
                    if (UnsubscribeMqttAsync().Result.IsBad())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Apply changes
                    wrappedNode.Value = trimmedWriteValue;

                    //Subscribe to new MQTT topic (sync)
                    if (SubscribeMqttAsync().Result.IsBad())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }
                    break;

                //target-address
                case Names.TargetAddress:
                    if (writeValue.IsEmptyOrWhiteSpace())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Apply changes
                    wrappedNode.Value = trimmedWriteValue;
                    break;


                //json-datatype
                case Names.JsonDataType:
                    if (writeValue.IsEmptyOrWhiteSpace())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Apply changes
                    wrappedNode.Value = trimmedWriteValue;
                    break;

                //quality-of-service
                case Names.QualityOfService:
                    if (!writeValue.IsNumber)
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    if (!MqttClientWrapper.IsValidQualityOfServiceLevel(writeValue.ToInt32()))
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Unsubscribe with current MQTT QoS (sync)
                    if (UnsubscribeMqttAsync().Result.IsBad())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }

                    //Apply changes
                    wrappedNode.Value = writeValue;

                    //Subscribe with new MQTT QoS (sync)
                    if (SubscribeMqttAsync().Result.IsBad())
                    {
                        result.SetResult(DLR_RESULT.DL_FAILED);
                        return;
                    }
                    break;
            }

            //Success
            result.SetResult(DLR_RESULT.DL_OK, writeValue);
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

        /// <summary>
        /// MQTT message received event handler
        /// </summary>
        /// <param name="args"></param>
        protected override async void OnMessageReceived(MqttApplicationMessageReceivedEventArgs args)
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

            //Filter for our topic
            var topic = GetNode(Names.Topic).Value.ToString();

            //Match (filter) for our topic
            if (!MqttTopicFilterComparer.IsMatch(args.ApplicationMessage.Topic, topic))
            {
                return;
            }

            var targetAddress = GetNode(Names.TargetAddress).Value.ToString();
            var jsonDataType = GetNode(Names.JsonDataType).Value.ToString();
            var stringifiedPayload = Encoding.UTF8.GetString(args.ApplicationMessage.Payload);

            //Convert to write variant 
            var (result, writeValue) = ToVariant(jsonDataType, stringifiedPayload);
            if (result.IsBad())
            {
                Console.WriteLine($"Write topic '{topic}' failed with {result}! {stringifiedPayload} ({jsonDataType}) -> '{targetAddress}'");
                return;
            }

            //Write the Value to the Data Layer
            var task = Root.Client.WriteAsync(targetAddress, writeValue);
            var taskResult = await task;
            if (taskResult.Result.IsBad())
            {
                Console.WriteLine($"Write topic '{topic}' failed with {taskResult.Result}! {writeValue.Value} ({writeValue.JsonDataType}) -> '{targetAddress}'");
                return;
            }

            //Success
            Console.WriteLine($"Write topic: '{topic}' -> {writeValue.Value} ({writeValue.JsonDataType}) -> '{targetAddress}'");
        }

        #endregion
    }
}
