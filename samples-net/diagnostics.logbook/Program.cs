/*
MIT License

Copyright (c) 2021 Bosch Rexroth AG

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
