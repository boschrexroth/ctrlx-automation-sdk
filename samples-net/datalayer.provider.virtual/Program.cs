/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;
using System;
using System.Threading.Tasks;

namespace Samples.Datalayer.Provider.Virtual
{
    class Program
    {
        /// <summary>
        /// The Main method is the entry point of an executable app.
        /// </summary>
        /// <param name="args">The args<see cref="string"/>.</param>
        static async Task Main(string[] args)
        {
            // Create TaskCompletionSource to wait for process termination  
            var tcs = new TaskCompletionSource();

            // Handle process exit event (SIGTERM)
            AppDomain.CurrentDomain.ProcessExit += (_, _) =>
            {
                Console.WriteLine("Process exit event SIGTERM received.");

                // Run task for graceful shutdown
                tcs.SetResult();
            };

            // Create a new ctrlX Data Layer system
            using var system = new DatalayerSystem();

            // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX CORE
            system.Start(startBroker: false);
            Console.WriteLine("ctrlX Data Layer system started.");

            // Create a remote address with the parameters according to your environment
            var remote = new Remote(ip: "192.168.1.1", sslPort: 443).ToString();

            // Create the provider with remote connection string
            using var provider = system.Factory.CreateProvider(remote);
            Console.WriteLine("ctrlX Data Layer provider created.");

            //Create root node handler
            var virtualHandler = new VirtualNodeHandler(provider, "sdk/net/provider/virtual");

            //Start the handler
            if (virtualHandler.Start().IsBad())
            {
                //if not we exit and retry after app daemon restart-delay (see snapcraft.yaml)
                Console.WriteLine($"Restarting app after restart-delay of 10 s ...");
                return;
            }

            // Wait for process termination
            Console.WriteLine("Waiting for process exit event 'SIGTERM'...");
            await tcs.Task;
            Console.WriteLine("Graceful shutdown app");

            // Stop the provider
            provider.Stop();
            Console.WriteLine("Provider stopped.");

            // Stop the ctrlX Data Layer system
            system.Stop();
            Console.WriteLine("ctrlX Data Layer system stopped.");
        }
    }
}
