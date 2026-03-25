/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
using System;
using System.Globalization;
using System.Threading.Tasks;

// Create TaskCompletionSource to wait for process termination. 
var tcs = new TaskCompletionSource();

// Handle process exit event (SIGTERM).
AppDomain.CurrentDomain.ProcessExit += (_, _) =>
{
    Console.WriteLine("Process exit event SIGTERM received.");

    // Run task for graceful shutdown.
    tcs.SetResult();
};

var varName = "DOTNET_SYSTEM_GLOBALIZATION_INVARIANT";
var value = Environment.GetEnvironmentVariable(varName);
Console.WriteLine($"{varName} = {value ?? "(not set)"}");

// Dump example date and number formats.
var today = DateTime.Now;

// Example number
double amount = 12345.67;

// Cultures to test
string[] cultures = ["en-US", "fr-FR", "de-DE", "ja-JP", "ar-SA"];

foreach (var cultureName in cultures)
{
    var culture = new CultureInfo(cultureName);

    Console.WriteLine($"--- Culture: {culture.DisplayName} ({culture.Name}) ---");

    // Date formatting.
    Console.WriteLine("Date: " + today.ToString("D", culture));

    // Currency formatting.
    Console.WriteLine("Currency: " + amount.ToString("C", culture));

    // Number formatting with thousands separator.
    Console.WriteLine("Number: " + amount.ToString("N", culture));

    Console.WriteLine();
}

// Wait for process termination.
Console.WriteLine("Waiting for process exit event 'SIGTERM'...");
await tcs.Task;
Console.WriteLine("Graceful shutdown.");