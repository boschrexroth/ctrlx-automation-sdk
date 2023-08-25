/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

namespace Samples.Diagnostics.Logbook
{
    using System;

    /// <summary>
    /// Defines the <see cref="Program" />.
    /// </summary>
    class Program
    {
        /// <summary>
        /// The Main method is the entry point of an executable app.
        /// </summary>
        /// <param name="args">The args<see cref="string"/>.</param>
        static void Main(string[] args)
        {
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
        }
    }
}
