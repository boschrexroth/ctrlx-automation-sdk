/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package datalayer.provider.sub;

import java.lang.System.Logger.Level;
import java.util.concurrent.TimeUnit;

import com.boschrexroth.api.Client;
import com.boschrexroth.api.Subscription;
import com.boschrexroth.api.SubscriptionPropertiesBuilder;
import com.boschrexroth.api.Variant;

public class RunClient {

    private static final java.lang.System.Logger LOG = java.lang.System
            .getLogger(RunClient.class.getName());

    public static void run(Client client) {
        SubsCallback callback = new SubsCallback();
        SubscriptionPropertiesBuilder props = SubscriptionPropertiesBuilder
                .create("test_id");
        Variant vprops = props.build();
        try (Subscription sub = client.createSubscription(vprops, callback)) {

            sub.subscribe(new String[] { ProviderNodeCallbackFolder.addressfolderRoot + "int1" });
            sub.subscribe(new String[] { ProviderNodeCallbackFolder.addressfolderRoot + "int2" });

            while (RunUtils.getRunningFlag().get()) {
                if (!client.isConnected()) {
                    LOG.log(Level.ERROR, "connecting to failed");
                    return;
                }
                TimeUnit.SECONDS.sleep(2);
            }

            sub.unsubscribeAll();

        } catch (Exception e) {
            Thread.currentThread().interrupt();
            LOG.log(Level.INFO, "Main thread interrupted. End of loop.");
        }
    }

}
