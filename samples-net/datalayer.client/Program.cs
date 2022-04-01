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
using System.Threading;

namespace Samples.Datalayer.Client
{
    class Program
    {
        // This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:DATALAYER_PORT?sslport=SSL_PORT
        // Please check and change according your environment:
        // - USER:        Enter your user name here - default is boschrexroth
        // - PASSWORD:    Enter your password here - default is boschrexroth
        // - IP_ADDRESS:
        //   127.0.0.1    If you develop on your (Windows) host and you want to connect to a ctrlX CORE virtual with port forwarding
        //   10.0.2.2     If you develop on a VM (QEMU, Virtual Box) and you want to connect to a ctrlX virtual with port forwarding
        //   192.168.1.1  If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater
        // - DATALAYER_PORT:
        //   2069         The ctrlX Data Layer client port
        //   2070         The ctrlX Data Layer provider port
        // - SSL_PORT:
        //   443          Default SSL Port if you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater
        //   8443         Default forwarded SSL Port if you are using a ctrlX CORE virtual

        // Please change the following constants according to your environment
        private const string USER = "boschrexroth";
        private const string PASSWORD = "boschrexroth";
        private const string IP_ADDRESS = "10.0.2.2";
        private const int SSL_PORT = 8443;

        static void Main(string[] args)
        {
            //Add app exit handler to handle optional clean up
            AppDomain.CurrentDomain.ProcessExit += CurrentDomain_ProcessExit;

            // Create a new ctrlX Data Layer system
            using var system = new DatalayerSystem();

            // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
            system.Start(startBroker: false);
            Console.WriteLine("ctrlX Data Layer system started.");

            // Check if the process is running inside a snap 
            var isSnapped = !string.IsNullOrEmpty(Environment.GetEnvironmentVariable("SNAP"));
            Console.WriteLine($"Running inside snap: {isSnapped}");

            // Set remote connection string with ipc protocol if running in snap, otherwise with tcp protocol
            var remote = isSnapped ? "ipc://" : $"tcp://{USER}:{PASSWORD}@{IP_ADDRESS}:2069?sslport={SSL_PORT}";

            // Create the client with remote connection string
            using var client = system.Factory.CreateClient(remote);
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
                        Console.WriteLine($"{memUsed}: {valueMemUsed.ToFloat()} (single read)");
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
