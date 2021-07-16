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
    using Microsoft.Extensions.Logging;

    /// <summary>
    /// Defines the <see cref="MicrosoftLoggerSample" />.
    /// </summary>
    internal class MicrosoftLoggerSample
    {
        /// <summary>
        /// The Log.
        /// </summary>
        public void Log()
        {
            var logger = MicrosoftLoggerFactory.GetInstance().CreateLogger<MicrosoftLoggerSample>();
            using (logger.BeginScope("[scope1]"))
            {
                // Write log message via Microsoft.Extensions.Logging with level 'Critical'
                logger.LogCritical("Critical via Microsoft.Extensions.Logging");

                // Write log message via Microsoft.Extensions.Logging with level 'Error'
                logger.LogError("Error via Microsoft.Extensions.Logging");

                // Write log message via Microsoft.Extensions.Logging with level 'Warning'
                logger.LogWarning("Warning via Microsoft.Extensions.Logging");
            }

            using (logger.BeginScope("[scope2]"))
            {
                // Write log message via Microsoft.Extensions.Logging with level 'Info'
                logger.LogInformation("Information via Microsoft.Extensions.Logging");

                // Write log message via Microsoft.Extensions.Logging with level 'Debug'
                logger.LogDebug("Debug via Microsoft.Extensions.Logging");

                // Write log message via Microsoft.Extensions.Logging with level 'Trace'
                logger.LogTrace("Trace via Microsoft.Extensions.Logging");
            }
        }
    }
}
