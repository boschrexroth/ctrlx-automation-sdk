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
using System;
using System.Diagnostics;
using System.Net;

namespace Samples.Datalayer.Client.Browse
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

            // Check if client is connected.
            Console.WriteLine($"Client connected: {client.IsConnected}");

            if (!client.IsConnected)
            {
                //if not we exit and retry after app daemon restart-delay (see snapcraft.yaml)
                Console.WriteLine($"Restarting app after restart-delay of 10 s ...");
                return;
            }

            if (client.IsConnected)
            {
                var stopWatch = new Stopwatch();
                stopWatch.Start();

                // Traverse the whole ctrlX Data Layer tree
                client.BrowseTree(system.Converter);

                stopWatch.Stop();

                Console.WriteLine("Elapsed time for browsing ctrlX Data Layer tree: {0} milliseconds", stopWatch.ElapsedMilliseconds);
            }
            else
            {
                Console.WriteLine("Client is disconnected: skip browsing ctrlX Data Layer tree.");
            }

            // Stop the ctrlX Data Layer system
            system.Stop();
            Console.WriteLine("ctrlX Data Layer system stopped.");
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
    }
}
