/*
MIT License

Copyright (c) 2021-2022 Bosch Rexroth AG

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
    internal class KernelLoggerSample
    {
        public void Log()
        {
            // Write log message via KernelLogger with level 'Alert'
            KernelLogger.LogAlert("Alert via KernelLogger");

            // Write log message via KernelLogger with level 'Critical'
            KernelLogger.LogCritical("Critical via KernelLogger");

            // Write log message via KernelLogger with level 'Error'
            KernelLogger.LogError("Error via KernelLogger");

            // Write log message via KernelLogger with level 'Warning'
            KernelLogger.LogWarning("Warning via KernelLogger");

            // Write log message via KernelLogger with level 'Info'
            KernelLogger.LogNotice("Notice via KernelLogger");

            // Write log message via KernelLogger with level 'Info'
            KernelLogger.LogInformation("Information via KernelLogger");

            // Write log message via KernelLogger with level 'Debug'
            KernelLogger.LogDebug("Debug via KernelLogger");
        }
    }
}
