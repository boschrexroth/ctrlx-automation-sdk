/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

namespace Samples.Diagnostics.Logbook
{
    using System;
    using System.Diagnostics;
    using System.Runtime.InteropServices;

    /// <summary>
    /// Defines the <see cref="KernelLogger" />.
    /// </summary>
    internal static class KernelLogger
    {
        /// <summary>
        /// Defines IsLinux, true if OS == linux..
        /// </summary>
        private static readonly bool IsLinux = RuntimeInformation.IsOSPlatform(OSPlatform.Linux);

        /// <summary>
        /// Logs the message to linux kernel log if os == linux.
        /// </summary>
        /// <param name="level">The level<see cref="KernelLogLevel"/>.</param>
        /// <param name="message">The message<see cref="string"/>.</param>
        public static void Log(KernelLogLevel level, string message)
        {
            if (IsLinux)
            {
                var levelAsInt = (int)level;
                var arguments = $"-p {levelAsInt} {message}";
                Process.Start("logger", arguments);
            }
            else
            {
                Console.WriteLine($"{DateTime.UtcNow:yyyy-MM-dd HH:mm:ss.fff} [{level}] {message}");
            }
        }

        public static void LogEmergency(string message)
        {
            Log(KernelLogLevel.Emergency, message);
        }

        /// <summary>
        /// The LogAlert.
        /// </summary>
        /// <param name="message">The message<see cref="string"/>.</param>
        public static void LogAlert(string message)
        {
            Log(KernelLogLevel.Alert, message);
        }

        /// <summary>
        /// The LogCritical.
        /// </summary>
        /// <param name="message">The message<see cref="string"/>.</param>
        internal static void LogCritical(string message)
        {
            Log(KernelLogLevel.Critical, message);
        }

        /// <summary>
        /// The LogError.
        /// </summary>
        /// <param name="message">The message<see cref="string"/>.</param>
        internal static void LogError(string message)
        {
            Log(KernelLogLevel.Error, message);
        }

        /// <summary>
        /// The LogWarning.
        /// </summary>
        /// <param name="message">The message<see cref="string"/>.</param>
        internal static void LogWarning(string message)
        {
            Log(KernelLogLevel.Warning, message);
        }

        /// <summary>
        /// The LogInformation.
        /// </summary>
        /// <param name="message">The message<see cref="string"/>.</param>
        internal static void LogInformation(string message)
        {
            Log(KernelLogLevel.Info, message);
        }

        /// <summary>
        /// The LogDebug.
        /// </summary>
        /// <param name="message">The message<see cref="string"/>.</param>
        internal static void LogDebug(string message)
        {
            Log(KernelLogLevel.Debug, message);
        }

        /// <summary>
        /// The LogNotice.
        /// </summary>
        /// <param name="message">The message<see cref="string"/>.</param>
        public static void LogNotice(string message)
        {
            Log(KernelLogLevel.Notice, message);
        }


    }
}
