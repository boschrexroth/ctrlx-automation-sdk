/*
MIT License

Copyright (c) 2021-2022 Bosch Rexroth AG

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
using System;
using System.Threading;
using System.Threading.Tasks;

namespace Samples.Datalayer.Client
{
    class Program
    {
        /// <summary>
        /// The Main method is the entry point of an executable app.
        /// </summary>
        /// <param name="args">The args<see cref="string"/>.</param>
        static async Task Main(string[] args)
        {
            // Create a new ctrlX Data Layer system
            using var system = new DatalayerSystem();

            // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
            system.Start(startBroker: false);
            Console.WriteLine("ctrlX Data Layer system started.");

            // Create a connection string with the parameters according to your environment (see Remote class)
            var connectionString = new Remote(ip: "192.168.1.1", sslPort: 443).ToString();

            // Create the client with remote connection string
            using var client = system.Factory.CreateClient(connectionString);
            Console.WriteLine("ctrlX Data Layer client created.");

            // Check if client is connected
            if (!client.IsConnected)
            {
                // We exit and retry after app daemon restart-delay (see snapcraft.yaml)
                Console.WriteLine($"Client is not connected -> exit");
                return;
            }

            // Define the subscription properties by using helper class SubscriptionPropertiesBuilder.
            var propertiesFlatbuffers = new SubscriptionPropertiesBuilder("mySubscription")
                .SetKeepAliveIntervalMillis(10000)
                .SetPublishIntervalMillis(1000)
                .SetErrorIntervalMillis(1000)
                .Build();


            // Define the subscription properties by using Flatbuffers class SubscriptionProperties. 
            // var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
            // var properties = SubscriptionProperties.CreateSubscriptionProperties(
            //    builder: builder,
            //    idOffset: builder.CreateString("mySubscription"),
            //    keepaliveInterval: 10000,
            //    publishInterval: 1000,
            //    errorInterval: 10000);
            // builder.Finish(properties.Value);
            // var propertiesFlatbuffers = new Variant(builder);

            // Create the subscription
            var (createResult, subscription) = client.CreateSubscription(propertiesFlatbuffers, userData: null);
            if (createResult.IsBad())
            {
                Console.WriteLine($"Failed to create subscription: {createResult}");
                return;
            }

            // Add DataChanged Event Handler
            subscription.DataChanged += (subscription, eventArgs) =>
            {
                var notifyInfo = NotifyInfo.GetRootAsNotifyInfo(eventArgs.Item.Info.ToFlatbuffers());
                var timestampUtc = DateTime.FromFileTimeUtc(Convert.ToInt64(notifyInfo.Timestamp));
                Console.WriteLine($"{timestampUtc}, {notifyInfo.Node}: {eventArgs.Item.Value.ToFloat()} (subscription)");
            };

            // Subscribe the Node with address 'framework/metrics/system/cpu-utilisation-percent'
            const string cpuLoad = "framework/metrics/system/cpu-utilisation-percent";
            var subscribeResult = subscription.Subscribe(address: cpuLoad);
            if (subscribeResult.IsBad())
            {
                Console.WriteLine($"Failed to subscribe: {subscribeResult}");
                return;
            }

            // Just keep the process running
            while (true)
            {
                if (client.IsConnected)
                {
                    // Read a node with address 'framework/metrics/system/memused-percent' without subscription (single read)
                    const string memUsed = "framework/metrics/system/memused-percent";

                    //We're using the async read method just for demonstration here.
                    //Hint: If you're not using any async calls, you can remove async keyword from the Main() method signature.
                    var task = client.ReadAsync(address: memUsed);
                    var taskResult = await task;

                    if (taskResult.Result.IsGood())
                    {
                        Console.WriteLine($"{memUsed}: {taskResult.Value.ToFloat()} (single read)");
                    }
                }

                Thread.Sleep(10000);
            }
        }
    }
}
