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
using System;


namespace Samples.HelloWorld
{
    class Program
    {
        static void Main(string[] args)
        {
            //Add app exit handler to handle optional clean up
            AppDomain.CurrentDomain.ProcessExit += CurrentDomain_ProcessExit;

            while (true)
            {
                Console.WriteLine("Hello World! {0}", DateTime.Now);
                System.Threading.Thread.Sleep(1000);
            }
        }

        private static void CurrentDomain_ProcessExit(object sender, EventArgs e)
        {
            Console.WriteLine("Application exit");

            //The EventHandler for this event can perform termination activities, such as closing files, releasing storage and so on, before the process ends.

            //Note:
            //In.NET Framework, the total execution time of all ProcessExit event handlers is limited,
            //just as the total execution time of all finalizers is limited at process shutdown.
            //The default is two seconds. An unmanaged host can change this execution time by calling the ICLRPolicyManager::SetTimeout method with the OPR_ProcessExit enumeration value.
            //This time limit does not exist in .NET Core.

            // Your optional clean up code goes here ... 
            //...
        }
    }
}
