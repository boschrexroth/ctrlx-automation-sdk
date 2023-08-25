/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
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
