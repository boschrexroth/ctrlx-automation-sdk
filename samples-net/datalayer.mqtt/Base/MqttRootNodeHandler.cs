/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;
using Samples.Datalayer.MQTT.Client;
using Samples.Datalayer.MQTT.Pub;
using Samples.Datalayer.MQTT.Sub;
using Samples.Datalayer.MQTT.Test;
using System;

using System.Threading;

namespace Samples.Datalayer.MQTT.Base
{
    /// <summary>
    /// Handler for root node
    /// </summary>
    internal class MqttRootNodeHandler : MqttBaseNodeHandler
    {
        /// <summary>
        ///  Creates a handler
        /// </summary>
        /// <param name="client"></param>
        /// <param name="provider"></param>
        /// <param name="brokerAddress"></param>
        /// <param name="baseAddress"></param>
        /// <param name="name"></param>
        public MqttRootNodeHandler(IClient client, IProvider provider, string brokerAddress, string baseAddress, string name) :
                base(baseAddress, name)
        {
            Client = client;
            Provider = provider;
            MqttClient = new MqttClientWrapper() { BrokerAddress = brokerAddress };
        }

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
        /// Gets the ctrlX Data Layer client 
        /// </summary>
        public IClient Client { get; private set; }

        /// <summary>
        /// Gets the MQTT client
        /// </summary>
        public MqttClientWrapper MqttClient { get; private set; }

        /// <summary>
        /// Gets the Test Handler
        /// </summary>
        public MqttTestNodeHandler Test { get; private set; }

        #endregion

        #region Overrides

        /// <summary>
        /// Starts the handler
        /// </summary>
        /// <returns></returns>
        public override DLR_RESULT Start()
        {
            //Wait until client connected
            if (WaitUntilConnected(Client, TimeoutMillis).IsBad())
            {
                Console.WriteLine("ctrlX Data Layer client could not be connected!");
                return DLR_RESULT.DL_FAILED;
            }

            //Create child handlers and add
            Handlers.Add(new MqttClientNodeHandler(this, this));
            Handlers.Add(new MqttPubNodeHandler(this, this));
            Handlers.Add(new MqttSubNodeHandler(this, this));
            Handlers.Add(new MqttSubScannerNodeHandler(this, this));

            //Create a TestHandler with dummy nodes
            Test = new MqttTestNodeHandler(this, this);
            Handlers.Add(Test);

            // Start the Provider
            if (Provider.Start().IsBad())
            {
                Console.WriteLine("ctrlX Data Layer provider could not be started!");
                return DLR_RESULT.DL_FAILED;
            }

            //Folder (self)
            var (result, node) = Provider.CreateBranchNode(this, BaseAddress, Name);
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            return base.Start();
        }

        /// <summary>
        /// Stops the handler
        /// </summary>
        /// <returns></returns>
        public override DLR_RESULT Stop()
        {
            // Stop ctrlX Data Layer provider
            var result = Provider.Stop();
            Console.Write(value: $"Provider stopped: {result}");


            //Unlock application
            Lock.Set();

            return base.Stop();
        }

        #endregion

        #region Private

        /// <summary>
        /// Waits until the client is connected
        /// </summary>
        /// <param name="client"></param>
        /// <param name="timeoutMillis"></param>
        private static DLR_RESULT WaitUntilConnected(IClient client, uint timeoutMillis)
        {
            //Wait until connected
            var start = DateTime.Now;

            while (!client.IsConnected)
            {
                Thread.Sleep(100);

                if (DateTime.Now.Subtract(start).TotalMilliseconds > timeoutMillis)
                {
                    Console.WriteLine("Timeout waiting for client connection!");
                    return DLR_RESULT.DL_TIMEOUT;
                }
            }

            return DLR_RESULT.DL_OK;
        }

        #endregion
    }
}
