/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;
using System;

namespace Samples.Datalayer.Client.Browse
{
    class Program
    {
        /// <summary>
        /// The Main method is the entry point of an executable app.
        /// </summary>
        /// <param name="args">The args<see cref="string"/>.</param>
        static void Main(string[] args)
        {
            // Create a new ctrlX Data Layer system
            using var system = new DatalayerSystem();

            // Starts the ctrlX Data Layer system without a new broker (startBroker = false) because one broker is already running on ctrlX CORE
            system.Start(startBroker: false);
            Console.WriteLine("ctrlX Data Layer system started.");

            // Create a remote address with the parameters according to your environment
            var remote = new Remote(ip: "192.168.1.1", sslPort: 443).ToString();

            // Create the client with remote connection string
            using var client = system.Factory.CreateClient(remote);
            Console.WriteLine("ctrlX Data Layer client created.");

            // Check if client is connected
            if (!client.IsConnected)
            {
                // We exit and retry after app daemon restart-delay (see snapcraft.yaml)
                Console.WriteLine($"Client is not connected -> exit");
                return;
            }

            // Traverse the whole ctrlX Data Layer tree
            client.BrowseTree();

            // Stop the ctrlX Data Layer system
            system.Stop();
            Console.WriteLine("ctrlX Data Layer system stopped.");
        }

    }
}
