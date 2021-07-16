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
using System;
using System.Net;
using System.Threading;

namespace Samples.Datalayer.Client
{
    class Program
    {
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!! CHANGE THIS TO YOUR ENVIRONMENT !!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        private static readonly IPAddress IpAddress = IPAddress.Parse("192.168.1.1"); // IPAddress.Loopback
        private static readonly string Username = "boschrexroth";
        private static readonly string Password = "boschrexroth";

        static void Main(string[] args)
        {
            //Add app exit handler to handle optional clean up
            AppDomain.CurrentDomain.ProcessExit += CurrentDomain_ProcessExit;


            // Check if the process is running inside a snap 
            var isSnapped = !string.IsNullOrEmpty(Environment.GetEnvironmentVariable("SNAP"));
            Console.WriteLine($"Running inside snap: {isSnapped}");

            // Create a new ctrlX Data Layer system
            using var system = new DatalayerSystem();

            // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
            system.Start(startBroker: false);
            Console.WriteLine("ctrlX Data Layer system started.");

            // Create the client with inter-process communication (ipc) protocol if running in snap, otherwise tcp
            using var client = isSnapped
                ? system.Factory.CreateIpcClient()
                : system.Factory.CreateTcpClient(IpAddress,
                    DatalayerSystem.DefaultClientPort,
                    Username,
                    Password);
            Console.WriteLine("ctrlX Data Layer client created.");

            if (WaitUntilConnected(client, 10000).IsBad())
            {
                //if not we exit and retry after app daemon restart-delay (see snapcraft.yaml)
                Console.WriteLine($"Restarting app after restart-delay of 10 s ...");
                return;
            }
            // Check if client is connected
            Console.WriteLine($"Client connected: {client.IsConnected}");

            // Define the subscription properties by using Flatbuffers class SubscriptionProperties. 
            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
            var properties = SubscriptionProperties.CreateSubscriptionProperties(
                builder: builder,
                idOffset: builder.CreateString("mySubscription"),
                keepaliveInterval: 10000,
                publishInterval: 1000,
                errorInterval: 10000);
            builder.Finish(properties.Value);
            var propertiesFlatbuffers = new Variant(builder);

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
                    // The node with address 'framework/metrics/system/memused-percent' we want to read without subscription
                    const string memUsed = "framework/metrics/system/memused-percent";

                    // Read value of the node 'memUsed' about every 10 s.
                    var (readResultMemUsed, valueMemUsed) = client.Read(address: memUsed);
                    if (readResultMemUsed.IsGood())
                    {
                        Console.WriteLine($"{valueMemUsed.Timestamp}, {memUsed}: {valueMemUsed.ToFloat()} (single read)");
                    }
                }
                else
                {
                    Console.WriteLine("Client is disconnected: skip reading value.");
                }

                Thread.Sleep(10000);
            }
        }

        private static void CurrentDomain_ProcessExit(object sender, EventArgs e)
        {
            Console.WriteLine("Application exit");

            //The EventHandler for this event can perform termination activities, such as closing files, releasing storage and so on, before the process ends.

            //Note:
            //In.NET Framework, the total execution time of all ProcessExit event handlers is limited,
            //just as the total execution time of all finalizers is limited at process shutdown.
            //The default is two seconds. An unmanaged host can change this execution time by calling the ICLRPolicyManager::SetTimeout method with the OPR_ProcessExit enumeration value.
            //This time limit does not exist in .NET Core.

            // Your optional clean up code goes here ... 
            //...
        }

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
