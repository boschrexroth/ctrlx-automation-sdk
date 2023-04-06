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
