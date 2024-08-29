/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
using System.Globalization;

namespace Hello.Web.Asp.services
{
    using Datalayer;
    using System;
    using System.Text.Json;

    /// <summary>
    /// Defines the <see cref="DataLayerService" />.
    /// </summary>
    public class DataLayerService
    {
        /// <summary>
        /// Gets the Client.
        /// </summary>
        public static IClient Client { get; private set; }

        /// <summary>
        /// Gets the DatalayerSystem.
        /// </summary>
        public static IDatalayerSystem System { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="DataLayerService"/> class.
        /// </summary>
        static DataLayerService()
        {
            CreateClient();
        }

        /// <summary>
        /// The CreateClient.
        /// </summary>
        /// <returns>The <see cref="IClient"/>.</returns>
        private static void CreateClient()
        {
            // Create a new ctrlX Data Layer system
            System = new DatalayerSystem();

            // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX CORE
            System.Start(startBroker: false);

            if (!System.IsStarted)
            {
                Console.WriteLine("Failed to connect ctrlX Data Layer client!");
                return;
            }

            // Create a remote address with the parameters according to your environment
            var remote = new Remote(ip: "192.168.1.1", sslPort: 443).ToString();

            // Create a Datalayer Client instance and connect. Automatically reconnects if the connection is interrupted.
            Client = System.Factory.CreateClient(remote);

            if (!Client.IsConnected)
            {
                Console.WriteLine("Failed to connect ctrlX Data Layer client!");
            }
        }

        /// <summary>
        /// Reads a value from the address.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <returns>The <see cref="NodeValue"/>.</returns>
        public NodeValue ReadNodeValue(string address)
        {
            if (Client == null)
            {
                Console.WriteLine("Failed to read node value from ctrlX Data Layer client!");
                return null;
            }

            var (result, variant) = Client.Read(address);
            var node = new NodeValue
            {
                Address = address,
                Value = string.Format(CultureInfo.InvariantCulture, "{0}", variant.Value),
                Type = variant.JsonDataType,
                Result = result.ToString()
            };

            return node;
        }
    }

    /// <summary>
    /// Defines the <see cref="NodeValue" />.
    /// </summary>
    public class NodeValue
    {
        /// <summary>
        /// Gets or sets the Address.
        /// </summary>
        public string Address { get; set; }

        /// <summary>
        /// Gets or sets the Value.
        /// </summary>
        public string Value { get; set; }

        /// <summary>
        /// Gets or sets the Type.
        /// </summary>
        public string Type { get; set; }

        /// <summary>
        /// Gets or sets the Timestamp.
        /// </summary>
        public DateTime Timestamp { get; set; }

        /// <summary>
        /// Gets or sets the Result.
        /// </summary>
        public string Result { get; set; }

        /// <summary>
        /// The ToString.
        /// </summary>
        /// <returns>The <see cref="string"/>.</returns>
        public override string ToString()
        {
            return JsonSerializer.Serialize(this);
        }
    }
}
