using System.Globalization;

namespace Hello.Web.Asp.services
{
    using Datalayer;
    using System;
    using System.Net;
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
        /// <param name="ip">The ip<see cref="IPAddress"/>.</param>
        /// <param name="user">The user<see cref="string"/>.</param>
        /// <param name="password">The password<see cref="string"/>.</param>
        public DataLayerService(IPAddress ip, string user, string password)
        {
            Client = CreateClient(ip, user, password);
        }

        /// <summary>
        /// The CreateClient.
        /// </summary>
        /// <param name="ip">The ip<see cref="IPAddress"/>.</param>
        /// <param name="user">The user<see cref="string"/>.</param>
        /// <param name="password">The password<see cref="string"/>.</param>
        /// <returns>The <see cref="IClient"/>.</returns>
        private static IClient CreateClient(IPAddress ip, string user, string password)
        {
            // Check if the process is running inside a snap 
            var isSnapped = !string.IsNullOrEmpty(Environment.GetEnvironmentVariable("SNAP"));
            Console.WriteLine($"Running inside snap: {isSnapped}");

            // Create a new ctrlX Data Layer system
            var system = new DatalayerSystem();

            // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
            system.Start(startBroker: false);
            Console.WriteLine("ctrlX Data Layer system started.");

            // Create the client with inter-process communication (ipc) protocol if running in snap, otherwise tcp
            var client = isSnapped
                ? system.Factory.CreateIpcClient()
                : system.Factory.CreateTcpClient(ip,
                    DatalayerSystem.DefaultClientPort,
                    user,
                    password);

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
                Timestamp = variant.Timestamp,
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
