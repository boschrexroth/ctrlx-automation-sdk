/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package datalayer.provider.sub;

import java.lang.System.Logger.Level;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

import com.boschrexroth.api.Provider;

public class RunUtils {

    private static final java.lang.System.Logger LOG = java.lang.System
            .getLogger(RunUtils.class.getName());

    public static AtomicBoolean running = new AtomicBoolean(true);

    public static void waitHandler(Provider p) {
        try {

            while (running.get()) {
                if (!p.isConnected()) {
                    LOG.log(Level.INFO, "connecting to failed");
                    return;
                }
                TimeUnit.SECONDS.sleep(2);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            LOG.log(Level.INFO, "Main thread interrupted. End of loop");
        }
    }
}
