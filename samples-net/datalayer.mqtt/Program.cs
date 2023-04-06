/*
MIT License

Copyright (c) 2021-2023 Bosch Rexroth AG

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
using Samples.Datalayer.MQTT.Base;
using System;

namespace Samples.Datalayer.MQTT
{
    class Program
    {
        private static MqttRootNodeHandler _mqttRootNodeHandler;

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

            // Create a remote address with the parameters according to your environment
            var remote = new Remote(ip: "192.168.1.1", sslPort: 443).ToString();

            // Create the client with remote connection string
            using var client = system.Factory.CreateClient(remote);
            Console.WriteLine("ctrlX Data Layer client created.");

            // Create the provider with remote connection string
            using var provider = system.Factory.CreateProvider(remote);
            Console.WriteLine("ctrlX Data Layer provider created.");

            //Create root node handler
            _mqttRootNodeHandler = new MqttRootNodeHandler(client, provider, "127.0.0.1", "samples", "mqtt");

            //Start the handler
            if (_mqttRootNodeHandler.Start().IsBad())
            {
                //if not we exit and retry after app daemon restart-delay (see snapcraft.yaml)
                Console.WriteLine($"Restarting app after restart-delay of 10 s ...");
                return;
            }

            //We have to keep application alive
            //This can be done waiting for a mutex
            Console.WriteLine($"Waiting for cancellation ...");
            _mqttRootNodeHandler.Lock.WaitOne();
            Console.WriteLine($"App exiting");
        }
    }
}
