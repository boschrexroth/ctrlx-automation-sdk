/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Samples.AppData;
using System;
using System.Threading.Tasks;

// Create TaskCompletionSource to wait for process termination  
var tcs = new TaskCompletionSource();

// Handle process exit event (SIGTERM)
AppDomain.CurrentDomain.ProcessExit += (_, _) =>
{
    Console.WriteLine("Received 'SIGTERM' event.");

    // Run task for graceful shutdown
    tcs.SetResult();
};

//Create the application
var myApplication = new MyApplication();

//Start the application
if (!myApplication.Start())
{
    // Initially exit and retry after app restart-delay (see snapcraft.yaml)
    Console.WriteLine("Restarting app after restart-delay of 10 s ...");
    return;
}

// Wait for process termination
Console.WriteLine("Waiting for process exit event 'SIGTERM' ...");
await tcs.Task;
Console.WriteLine("Gracefully shutdown app");

//Stop the application
myApplication.Stop();
Console.WriteLine("App exiting");