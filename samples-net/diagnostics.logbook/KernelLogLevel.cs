/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

namespace Samples.Diagnostics.Logbook
{
    /// <summary>
    /// Defines the linux kernel log level.
    /// Documentation about linux kernel log levels can be found<see "https://en.wikipedia.org/wiki/Printk">here</see>
    /// </summary>
    public enum KernelLogLevel
    {
        /// <summary>
        /// An emergency condition; the system is probably dead
        /// </summary>
        Emergency = 0,

        /// <summary>
        /// A problem that requires immediate attention
        /// </summary>
        Alert = 1,

        /// <summary>
        /// A critical condition
        /// </summary>
        Critical = 2,

        /// <summary>
        /// An error condition
        /// </summary>
        Error = 3,

        /// <summary>
        /// A warning condition
        /// </summary>
        Warning = 4,

        /// <summary>
        /// A normal, but perhaps noteworthy, condition
        /// </summary>
        Notice = 5,

        /// <summary>
        /// An informational message
        /// </summary>
        Info = 6,

        /// <summary>
        /// A debug message, typically superfluous
        /// </summary>
        Debug = 7
    }
}
