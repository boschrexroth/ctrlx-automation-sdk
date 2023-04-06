/*
MIT License

Copyright (c) 2021-2023 Bosch Rexroth AG

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
        public IClient Client { get; }

        /// <summary>
        /// Initializes a new instance of the <see cref="DataLayerService"/> class.
        /// </summary>
        public DataLayerService()
        {
            Client = CreateClient();
        }

        /// <summary>
        /// The CreateClient.
        /// </summary>
        /// <returns>The <see cref="IClient"/>.</returns>
        private static IClient CreateClient()
        {
            // Create a new ctrlX Data Layer system
            var system = new DatalayerSystem();

            // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
            system.Start(startBroker: false);
            Console.WriteLine("ctrlX Data Layer system started.");

            // Create a remote address with the parameters according to your environment
            var remote = new Remote(ip: "192.168.1.1", sslPort: 443).ToString();

            // Create the client with remote connection string
            using var client = system.Factory.CreateClient(remote);
            Console.WriteLine("ctrlX Data Layer client created.");

            return client;
        }

        /// <summary>
        /// The ReadNodeValue.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <returns>The <see cref="NodeValue"/>.</returns>
        public NodeValue ReadNodeValue(string address)
        {
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
