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

            // Traverse the whole ctrlX Data Layer tree
            client.BrowseTree();

            // Stop the ctrlX Data Layer system
            system.Stop();
            Console.WriteLine("ctrlX Data Layer system stopped.");
        }

    }
}
