/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */


using Datalayer;
using System;

namespace Samples.Appdata
{
    class Program
    {
        static void Main(string[] args)
        {
            //Create the application
            var myApplication = new MyApplication();

            //Start the application
            if (myApplication.Start().IsBad())
            {
                //if not we exit and retry after app daemon restart-delay (see snapcraft.yaml)
                Console.WriteLine($"Restarting app after restart-delay of 10 s ...");
                return;
            }

            //We have to keep application alive
            //This can be done waiting for a mutex
            Console.WriteLine($"Waiting for cancellation ...");
            myApplication.Lock.WaitOne();

            //Stop the application
            myApplication.Stop();
            Console.WriteLine($"App exiting");
        }
    }
}
