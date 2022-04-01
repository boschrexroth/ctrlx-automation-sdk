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

namespace Samples.Datalayer.Provider.Virtual
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
            // Create a new ctrlX Data Layer system
            using var system = new DatalayerSystem();

            // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX device
            system.Start(startBroker: false);
            Console.WriteLine("ctrlX Data Layer system started.");

            // Check if the process is running inside a snap 
            var isSnapped = !string.IsNullOrEmpty(Environment.GetEnvironmentVariable("SNAP"));
            Console.WriteLine($"Running inside snap: {isSnapped}");

            // Set remote connection string for provider with inter-process communication (ipc) protocol if running in snap, otherwise tcp
            var remote = isSnapped ? "ipc://" : $"tcp://{USER}:{PASSWORD}@{IP_ADDRESS}:2070?sslport={SSL_PORT}";

            // Create the provider with remote connection string
            using var provider = system.Factory.CreateProvider(remote);
            Console.WriteLine("ctrlX Data Layer provider created.");

            //Create root node handler
            var virtualHandler = new VirtualNodeHandler(provider, "samples", "virtual");

            //Start the handler
            if (virtualHandler.Start().IsBad())
            {
                //if not we exit and retry after app daemon restart-delay (see snapcraft.yaml)
                Console.WriteLine($"Restarting app after restart-delay of 10 s ...");
                return;
            }

            //We have to keep application alive
            //This can be done waiting for a mutex
            Console.WriteLine($"Waiting for cancellation ...");
            virtualHandler.Lock.WaitOne();
            Console.WriteLine($"App exiting");
        }

    }
}
