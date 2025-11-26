/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package datalayer.provider.sub;

import java.lang.System.Logger.Level;
import java.util.concurrent.TimeUnit;

import com.boschrexroth.api.Client;
import com.boschrexroth.api.NotifyItem;
import com.boschrexroth.api.Result;
import com.boschrexroth.api.Subscription;
import com.boschrexroth.api.SubscriptionPropertiesBuilder;
import com.boschrexroth.api.Variant;
import com.boschrexroth.api.VariantType;

final class SubsCallback implements com.boschrexroth.api.SubscriptionCallback {

    private static final java.lang.System.Logger LOG = java.lang.System.getLogger(SubsCallback.class.getName());

    @Override
    public void onSubscription(Result results, NotifyItem[] items) {
        if (results != Result.OK) {
            LOG.log(Level.ERROR, "onSubscription error: {0}", results);
            return;
        }

        for (NotifyItem item : items) {
            if (item.getData().getType() == VariantType.INT32) {
                LOG.log(Level.INFO, "address: {0}, value: {1}", item.getAddress(), item.getData().getInt32());
                continue;
            }
            LOG.log(Level.INFO, "address: {0}, value<?>_type: {1}", item.getAddress(), item.getData().getType());
        }
    }
}

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

            while (RunUtils.running.get()) {
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
