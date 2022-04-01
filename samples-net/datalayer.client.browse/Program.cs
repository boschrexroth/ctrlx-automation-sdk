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

namespace Samples.Datalayer.Client.Browse
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
                client.BrowseTree();

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
