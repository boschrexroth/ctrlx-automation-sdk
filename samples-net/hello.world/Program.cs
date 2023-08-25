/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
using System;


namespace Samples.HelloWorld
{
    class Program
    {
        /// <summary>
        /// The Main method is the entry point of an executable app.
        /// </summary>
        /// <param name="args">The args<see cref="string"/>.</param>
        static void Main(string[] args)
        {
            while (true)
            {
                Console.WriteLine("Hello World! {0}", DateTime.Now);
                System.Threading.Thread.Sleep(1000);
            }
        }
    }
}
