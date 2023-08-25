/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

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

            // Just keep the process running
            while (true)
            {
                if (client.IsConnected)
                {
                    var addresses = new string[]
                    {
                        "framework/metrics/system/cpu-utilisation-percent",
                        "framework/metrics/system/memavailable-mb",
                        "framework/metrics/system/membuffers-mb",
                        "framework/metrics/system/memcache-mb",
                        "framework/metrics/system/memfree-mb",
                        "framework/metrics/system/memtotal-mb",
                        "framework/metrics/system/memused-percent"
                    };

                    Console.WriteLine("BulkRead");
                    var (result, items) = client.BulkRead(addresses);
                    if (result.IsGood())
                    {
                        foreach (var item in items)
                        {
                            Console.WriteLine($"address: {item.Address}, value: {item.Value.ToFloat()}, timestamp: {item.Timestamp}, result: {item.Result}");
                        }
                    }

                    //We're using the async bulk read method just for demonstration here.
                    //Hint: If you're not using any async calls, you can remove async keyword from the Main() method signature.
                    Console.WriteLine("BulkReadAsync");
                    var task = client.BulkReadAsync(addresses);
                    var taskResult = await task;

                    if (taskResult.Result.IsGood())
                    {
                        foreach (var item in items)
                        {
                            Console.WriteLine($"address: {item.Address}, value: {item.Value.ToFloat()}, timestamp: {item.Timestamp}, result: {item.Result}");
                        }
                    }
                }

                Thread.Sleep(10000);
            }
        }
    }
}
