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

            // Create a connection string with the parameters according to your environment (see Remote class)
            var connectionString = new Remote(ip: "192.168.1.1", sslPort: 443).ToString();

            // Create the client with remote connection string
            using var client = system.Factory.CreateClient(connectionString);
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
