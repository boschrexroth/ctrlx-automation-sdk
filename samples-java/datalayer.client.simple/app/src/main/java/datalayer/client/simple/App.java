/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

package datalayer.client.simple;

import comm.datalayer.MetadataT;

import java.util.Enumeration;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicBoolean;

import com.boschrexroth.api.Client;
import com.boschrexroth.api.ClientResponseCallback;
import com.boschrexroth.api.DlSystem;
import com.boschrexroth.api.RemoteBuilder;
import com.boschrexroth.api.Result;
import com.boschrexroth.api.Variant;

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

    private static void runSync(Client c) {
        Result res = c.pingSync();
        if (res != Result.OK) {
            error("pingSync");
            return;
        }

        String[] vals = c.Browse("/");
        if (vals.length == 0) {
            error("Browse");
            return;
        }

        Variant v = Variant.create();
        res = c.readSync("diagnosis/cfg/realtime/numbers", v);
        if (res != Result.OK) {
            error("readSync");
            return;
        }
        System.out.println("Run: syncRead=" + String.valueOf(v.getBool8()));
    }

    private static void runAsync(Client c) {
        pingAsync(c);
        readAsync(c);
        browseAsync(c);
    }

    private static void browseAsync(Client c) {
        CompletableFuture<Variant> futureResponseVariant = new CompletableFuture<>();
        CompletableFuture<Result> futureResponseResult = new CompletableFuture<>();

        ClientResponseCallback callback = (result, variant) -> {
            futureResponseVariant.complete(variant);
            futureResponseResult.complete(result);
        };

        Result res = c.browseAsync("", callback);
        if (res != Result.OK) {
            error("browseAsync");
            return;
        }

        try {
            Variant receivedVariant = futureResponseVariant.get(5, TimeUnit.SECONDS); // Wait up to 5 seconds
            Result receivedResult = futureResponseResult.get(5, TimeUnit.SECONDS);
            if (receivedResult != Result.OK || receivedVariant == null) {
                error("browseAsync: result: " + receivedResult);
            }
        } catch (InterruptedException | ExecutionException | TimeoutException e) {
            error("browseAsync: exception: " + e);
            return;
        }
    }

    private static void readAsync(Client c) {
        CompletableFuture<Variant> futureResponseVariant = new CompletableFuture<>();
        CompletableFuture<Result> futureResponseResult = new CompletableFuture<>();

        ClientResponseCallback callback = (result, variant) -> {
            futureResponseVariant.complete(variant);
            futureResponseResult.complete(result);
        };

        Variant arg = Variant.create();
        Result res = c.readAsync("framework/metrics/system/cpu-utilisation-percent", arg, callback);
        if (res != Result.OK) {
            error("readAsync");
            return;
        }

        try {
            Variant receivedVariant = futureResponseVariant.get(5, TimeUnit.SECONDS); // Wait up to 5 seconds
            Result receivedResult = futureResponseResult.get(5, TimeUnit.SECONDS);
            if (receivedResult != Result.OK || receivedVariant == null) {
                error("readAsync: result: " + receivedResult);
            }
            if (receivedVariant != null) {
                System.out.println("Run: asyncRead=" + String.valueOf(receivedVariant.getInt32()));
            }
        } catch (InterruptedException | ExecutionException | TimeoutException e) {
            error("readAsync: exception: " + e);
            return;
        }
    }

    private static void pingAsync(Client c) {
        CompletableFuture<Variant> futureResponseVariant = new CompletableFuture<>();
        CompletableFuture<Result> futureResponseResult = new CompletableFuture<>();

        ClientResponseCallback callback = (result, variant) -> {
            futureResponseVariant.complete(variant);
            futureResponseResult.complete(result);
        };

        Result res = c.pingAsync(callback);
        if (res != Result.OK) {
            error("pingAsync");
            return;
        }

        try {
            Variant receivedVariant = futureResponseVariant.get(5, TimeUnit.SECONDS); // Wait up to 5 seconds
            Result receivedResult = futureResponseResult.get(5, TimeUnit.SECONDS);
            if (receivedResult != Result.OK || receivedVariant == null) {
                error("pingAsync: result: " + receivedResult);
            }
        } catch (InterruptedException | ExecutionException | TimeoutException e) {
            error("pingAsync: exception: " + e);
            return;
        }
    }

    private static void run(Client c) {
        System.out.println("Run begin");

        if (!running.get()) {
            return;
        }

        runSync(c);
        runAsync(c);

        System.out.println("Run end");
    }

    private static void error(String fun) {
        System.out.println(fun + " failed");
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

                try {

                    while (running.get()) {
                        if (!client.isConnected()) {
                            System.out.println("connecting to " + remote + " failed");
                            return;
                        }
                        run(client);
                        TimeUnit.SECONDS.sleep(2);
                    }

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
