/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

namespace Samples.Diagnostics.Logbook
{
    using Microsoft.Extensions.Configuration;
    using Microsoft.Extensions.Logging;
    using System;

    /// <summary>
    /// Defines the <see cref="MicrosoftLoggerFactory" />.
    /// </summary>
    internal class MicrosoftLoggerFactory
    {
        /// <summary>
        /// Defines the _loggerFactory.
        /// </summary>
        private static ILoggerFactory _loggerFactory;

        /// <summary>
        /// The GetInstance.
        /// </summary>
        /// <returns>The <see cref="ILoggerFactory"/>.</returns>
        public static ILoggerFactory GetInstance()
        {
            if (_loggerFactory == null)
            {
                var config = new ConfigurationBuilder()
                    .SetBasePath(AppDomain.CurrentDomain.BaseDirectory)
                    .AddJsonFile("appsettings.json", true, true)
                    .Build();

                var loggingConfiguration = config.GetSection("Logging");
                _loggerFactory = LoggerFactory.Create(builder =>
                {
                    builder.AddSystemdConsole().AddConfiguration(loggingConfiguration);
                });
            }

            return _loggerFactory;
        }
    }
}
