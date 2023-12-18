/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using System;
using System.Threading.Tasks;
using Samples.Diagnostics.Logbook;

// Create TaskCompletionSource to wait for process termination  
var tcs = new TaskCompletionSource();

// Handle process exit event (SIGTERM)
AppDomain.CurrentDomain.ProcessExit += (_, _) =>
{
    Console.WriteLine("Process exit event SIGTERM received.");

    // Run task for graceful shutdown
    tcs.SetResult();
};

// Write message via Console.WriteLine (standard output)
Console.WriteLine("Console.WriteLine");

// Write message via Console.Error.WriteLine (standard error)
Console.Error.WriteLine("Console.Error.WriteLine");

// Write log messages via KernelLogger
var kernelLoggerSample = new KernelLoggerSample();
kernelLoggerSample.Log();

// Write log message via logger of "Microsoft.Extensions.Logging" package
var microsoftLoggerSample = new MicrosoftLoggerSample();
microsoftLoggerSample.Log();

// Wait for process termination
Console.WriteLine("Waiting for process exit event 'SIGTERM'...");
await tcs.Task;
Console.WriteLine("Graceful shutdown app");