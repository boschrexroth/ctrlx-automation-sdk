/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
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
