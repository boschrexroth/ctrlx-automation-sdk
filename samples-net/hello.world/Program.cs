/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
using System;
using System.Threading.Tasks;

// Create TaskCompletionSource to wait for process termination  
var tcs = new TaskCompletionSource();

// Handle process exit event (SIGTERM)
AppDomain.CurrentDomain.ProcessExit += (_, _) =>
{
    Console.WriteLine("Process exit event SIGTERM received.");

    // Run task for graceful shutdown
    tcs.SetResult();
};

Console.WriteLine("Hello World! {0}", DateTime.Now);

// Wait for process termination
Console.WriteLine("Waiting for process exit event 'SIGTERM'...");
await tcs.Task;
Console.WriteLine("Graceful shutdown app");