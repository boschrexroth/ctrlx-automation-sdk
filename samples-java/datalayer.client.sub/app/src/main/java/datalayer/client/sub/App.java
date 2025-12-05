/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

package datalayer.client.sub;

import comm.datalayer.MetadataT;

import java.util.Enumeration;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

import com.boschrexroth.api.Client;
import com.boschrexroth.api.DlSystem;
import com.boschrexroth.api.NotifyItem;
import com.boschrexroth.api.RemoteBuilder;
import com.boschrexroth.api.Result;
import com.boschrexroth.api.Subscription;
import com.boschrexroth.api.SubscriptionPropertiesBuilder;
import com.boschrexroth.api.Variant;
import com.boschrexroth.api.VariantType;

final class SubsCallback implements com.boschrexroth.api.SubscriptionCallback {

    @Override
    public void onSubscription(Result results, NotifyItem[] items) {
        if (results != Result.OK) {
            return;
        }

        for (NotifyItem item : items) {
            if (item.getData().getType() == VariantType.FLOAT64) {
                System.out.printf("address: %s, value: %f \n", item.getAddress(), item.getData().getFLOAT64());
                continue;
            }
            System.out.printf("address: %s, value: %d \n", item.getAddress(), item.getData().getUint32());
        }
    }

}

public class App {
    private static AtomicBoolean running = new AtomicBoolean(true);

    public boolean setupSystem() {
        MetadataT m = new MetadataT();
        Variant v = Variant.create();
        if (m != null && v != null) {
            v.setBool8(true);
            return true;
        }
        return false;
    }

    public static void main(String[] args) {

        getEnv();

        try (DlSystem s = DlSystem.create("")) {
            s.start(false);

            String remote = RemoteBuilder.create().IpVirtual().PortVirtual().build();
            System.out.println("start: " + remote);

            Runtime.getRuntime().addShutdownHook(new Thread(() -> {
                System.out.println("\nShutdown hook activated! Start cleanup...");
                running.set(false);
            }));

            try (Client client = s.Factory().createClient(remote)) {

                SubsCallback callback = new SubsCallback();
                SubscriptionPropertiesBuilder props = SubscriptionPropertiesBuilder
                        .create("test_id");
                Variant vprops = props.build();
                try (Subscription sub = client.createSubscription(vprops, callback)) {
                    String [] subs = new String[] {
                        "framework/metrics/system/cpu-utilisation-percent",
		                "framework/metrics/system/memused-percent"
                    };
                    sub.subscribe(subs);

                    while (running.get()) {
                        if (!client.isConnected()) {
                            System.out.println("connecting to " + remote + " failed");
                            return;
                        }
                        TimeUnit.SECONDS.sleep(2);
                    }

                    sub.unsubscribeAll();

                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    System.out.println("Main thread interrupted. End of loop.");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void getEnv() {
        System.out.println("All Enviroments ");
        Map<String, String> env = System.getenv();
        for (String envName : env.keySet()) {
            System.out.format("%s=%s%n", envName, env.get(envName));
        }

        System.out.println("\nAll Java Properties ");
        Properties props = System.getProperties();
        Enumeration propNames = props.propertyNames();
        for (; propNames.hasMoreElements();) {
            String key = propNames.nextElement().toString();
            System.out.println(key + " : " + props.getProperty(key));
        }
    }
}
