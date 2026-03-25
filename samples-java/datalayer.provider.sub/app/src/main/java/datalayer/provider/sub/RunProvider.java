/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package datalayer.provider.sub;

import java.lang.System.Logger.Level;

import com.boschrexroth.api.Provider;
import com.boschrexroth.api.ProviderNode;
import com.boschrexroth.api.Result;

public class RunProvider {

    private static final java.lang.System.Logger LOG = java.lang.System
            .getLogger(RunProvider.class.getName());

    public static void run(Provider p) {
        LOG.log(Level.INFO, "Run begin");

        if (!RunUtils.getRunningFlag().get()) {
            return;
        }
        ProviderNodeCallbackFolder callbackFolder = new ProviderNodeCallbackFolder();

        try (ProviderNode nodeFolder = ProviderNode.create(callbackFolder)) {
            Result res = p.registerNode(ProviderNodeCallbackFolder.addressfolder, nodeFolder);

            if (res != Result.OK) {
                return;
            }

            p.start();

            RunUtils.waitHandler(p);

        } catch (Throwable e) {
            LOG.log(Level.ERROR, e.getMessage(), e);
        }
        LOG.log(Level.INFO, "Run end");
    }
}
