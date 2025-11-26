/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

package datalayer.provider.simple;

import java.nio.file.Paths;
import java.util.Enumeration;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

import com.boschrexroth.api.DlSystem;
import com.boschrexroth.api.Provider;
import com.boschrexroth.api.ProviderNode;
import com.boschrexroth.api.RemoteBuilder;
import com.boschrexroth.api.Result;

public class App {
    private static AtomicBoolean running = new AtomicBoolean(true);

    private static final String addressRoot = "sdk/java/provider/simple/";
    private static final String typeaddressfbs = "types/" + addressRoot + "inertial-value";

    private static void waitHandler(Provider p) {
        try {

            while (running.get()) {
                if (!p.isConnected()) {
                    System.out.println("connecting to failed");
                    return;
                }
                TimeUnit.SECONDS.sleep(2);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            System.out.println("Main thread interrupted. End of loop");
        }
    }

    private static void run(Provider p) {
        System.out.println("Run begin");

        if (!running.get()) {
            return;
        }
        ProviderNodeCallbackString callbackString = new ProviderNodeCallbackString();
        ProviderNodeCallbackInteger callbackInteger = new ProviderNodeCallbackInteger();
        ProviderNodeCallbackTimestamp callbackTimestamp = new ProviderNodeCallbackTimestamp();
        ProviderNodeCallbackFbs callbackFbs = new ProviderNodeCallbackFbs(typeaddressfbs);

        try (ProviderNode node = ProviderNode.create(callbackString);
                ProviderNode nodeInteger = ProviderNode.create(callbackInteger);
                ProviderNode nodeTimestamp = ProviderNode.create(callbackTimestamp);
                ProviderNode nodeFbs = ProviderNode.create(callbackFbs)) {
            Result res = p.registerNode(addressRoot + "string", node);

            if (res != Result.OK) {
                System.out.println("Error: registerNode: " + addressRoot + "string");
            }

            res = p.registerNode(addressRoot + "int32", nodeInteger);

            if (res != Result.OK) {
                System.out.println("Error: registerNode: " + addressRoot + "int32");
            }

            res = p.registerNode(addressRoot + "timestamp", nodeTimestamp);

            if (res != Result.OK) {
                System.out.println("Error: registerNode: " + addressRoot + "timestamp");
            }

            String userDirectoryPath = System.getProperty("user.dir");
            String bfbsPath = Paths.get(userDirectoryPath, "bfbs", "sampleSchema.bfbs").toString();

            String snap = java.lang.System.getenv("SNAP");
            if (snap != null && !snap.equals("")) {
                bfbsPath = Paths.get(snap, "bfbs", "sampleSchema.bfbs").toString();
            }

            res = p.registerType(typeaddressfbs, bfbsPath);

            if (res != Result.OK) {
                System.out.println("Error: registerType: " + typeaddressfbs + "path: " + bfbsPath);
            }
            
            res = p.registerNode(addressRoot + "inertial-value", nodeFbs);

            if (res != Result.OK) {
                System.out.println("Error: registerNode: " + addressRoot + "inertial-value");
            }

            p.start();

            waitHandler(p);

        } catch (Throwable e) {
            e.printStackTrace();
        }
        System.out.println("Run end");
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

            try (Provider provider = s.Factory().createProvider(remote)) {

                run(provider);

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
        Enumeration<?> propNames = props.propertyNames();
        for (; propNames.hasMoreElements();) {
            String key = propNames.nextElement().toString();
            System.out.println(key + " : " + props.getProperty(key));
        }
    }
}
