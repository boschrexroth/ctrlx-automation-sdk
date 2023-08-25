/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;
using MQTTnet.Client;
using MQTTnet.Protocol;
using Samples.Datalayer.MQTT.Base;
using System;
using System.Threading.Tasks;

namespace Samples.Datalayer.MQTT.Sub
{
    /// <summary>
    /// Handler for Sub configuration base node
    /// </summary>
    internal abstract class MqttSubBaseNodeHandler : MqttBaseNodeHandler
    {
        //Fields
        private static uint _nextSubscriptionId;

        /// <summary>
        /// Creates the handler
        /// </summary>
        /// <param name="root"></param>
        /// <param name="parent"></param>
        /// <param name="name"></param>
        public MqttSubBaseNodeHandler(MqttRootNodeHandler root, MqttBaseNodeHandler parent, string name) :
            base(root, parent, name)
        {
            //Add MQTT Client connected event handler
            Root.MqttClient.Connected += async args =>
            {
                //Check if started
                if (!IsStarted)
                {
                    return;
                }

                //We have to resubscribe to our topic if the MQTT client has been reconnected to be unaware of CleanSession and QoS setup 
                var task = SubscribeMqttAsync();
                var result = await task;
            };
        }


        #region Abstract

        /// <summary>
        /// Returns the Topic
        /// </summary>
        protected abstract string Topic { get; }

        /// <summary>
        /// Returns the Quality
        /// </summary>
        protected abstract MqttQualityOfServiceLevel Quality { get; }

        /// <summary>
        /// MQTT message received event handler
        /// </summary>
        /// <param name="args"></param>
        protected abstract Task OnMessageReceived(MqttApplicationMessageReceivedEventArgs args);

        #endregion

        #region Overrides

        /// <summary>
        /// Starts the handler
        /// </summary>
        /// <returns></returns>
        public override DLR_RESULT Start()
        {
            //Create, register and add the handled nodes here
            //...

            //Add MQTT MessageReceived EventHandler
            Root.MqttClient.MessageReceived += OnMessageReceived;

            //Subscribe to MQTT topic (async)
            Task.Factory.StartNew(() => SubscribeMqttAsync().Result);

            return base.Start();
        }

        /// <summary>
        /// Stops the handler
        /// </summary>
        /// <returns></returns>
        public override DLR_RESULT Stop()
        {
            //Remove MQTT MessageReceived EventHandler
            Root.MqttClient.MessageReceived -= OnMessageReceived;

            //Unsubscribe from MQTT (sync)
            var result = UnsubscribeMqttAsync().Result;
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }

            return base.Stop();
        }

        #endregion

        #region Protected

        /// <summary>
        /// Returns a unique SubscriptionId
        /// </summary>
        protected uint SubscriptionId { get; } = ++_nextSubscriptionId; //  must be > 0

        /// <summary>
        /// Subscribes a MQTT Topic
        /// </summary>
        /// <returns></returns>
        protected async Task<DLR_RESULT> SubscribeMqttAsync()
        {
            //If not connected, we subscribe later
            if (!Root.MqttClient.IsConnected)
            {
                return DLR_RESULT.DL_OK;
            }

            //Subscribe
            var task = Root.MqttClient.SubscribeAsync(Topic, SubscriptionId, Quality);
            var result = await task;
            return result;
        }

        /// <summary>
        /// Unsubscribes a MQTT topic
        /// </summary>
        /// <returns></returns>
        protected async Task<DLR_RESULT> UnsubscribeMqttAsync()
        {
            if (!Root.MqttClient.IsConnected)
            {
                return DLR_RESULT.DL_OK;
            }

            //We have to check if we can unsubscribe the topic
            if (IsSubscriptionInUse())
            {
                Console.WriteLine($"Unsubscribe skipped, because topic '{Topic}' is subscribed by another configuration.");
                return DLR_RESULT.DL_OK;
            }

            //Unsubscribe the topic
            var task = Root.MqttClient.UnsubscribeAsync(Topic);
            var result = await task;
            return result;
        }

        /// <summary>
        /// Indicates whether the topic can be unsubscribed
        /// </summary>
        /// <returns></returns>
        private bool IsSubscriptionInUse()
        {
            //We have to check if our topic is not used by another Sub handler
            return Parent.Handlers.Exists(handler => handler != this && handler.HasNode(Names.Topic) && handler.GetNode(Names.Topic).Value.ToString() == Topic);
        }

        #endregion
    }
}
