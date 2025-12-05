/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

package datalayer.client.bulk;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicBoolean;

import com.boschrexroth.api.Bulk;
import com.boschrexroth.api.BulkReadArg;
import com.boschrexroth.api.BulkResponse;
import com.boschrexroth.api.BulkResponseCallback;
import com.boschrexroth.api.Client;
import com.boschrexroth.api.DlSystem;
import com.boschrexroth.api.RemoteBuilder;
import com.boschrexroth.api.Result;
import comm.datalayer.MetadataT;

public class App {
    private static AtomicBoolean running = new AtomicBoolean(true);

    private static void runSync(Client c) {
        try (Bulk bulk = c.createBulk()) {

            bulkRead(bulk);
            bulkMetadata(bulk);
            bulkBrowse(bulk);

        } catch (Throwable e) {
            e.printStackTrace();
        }
    }

    private static void bulkBrowse(Bulk bulk) {
        System.out.println("Browse:");
        String[] reqs = new String[] { "framework/metrics/system",
                "framework/metrics/process" };
        Result res = bulk.browse(reqs);
        if (res != Result.OK) {
            error("bulkBrowse");
            return;
        }
        System.out.println("Browse Results:");
        for (BulkResponse resp : bulk.getResponses()) {
            if (resp.getResult() != Result.OK) {
                System.out.println(
                        "Address: " + resp.getAddress() + " : error:" + resp.getResult());
                continue;
            }
            String commaSeparated = String.join(", ", resp.getData().getArrayOfStrings());
            System.out.println(
                    "Address: " + resp.getAddress() + " : values: '" + commaSeparated + "'");
        }
    }

    private static void bulkMetadata(Bulk bulk) {
        System.out.println("Metadata:");
        String[] reqs = new String[] { "framework/metrics/system/cpu-utilisation-percent",
                "framework/metrics/system/memavailable-mb" };
        Result res = bulk.metadata(reqs);
        if (res != Result.OK) {
            error("bulkMetadata");
            return;
        }
        System.out.println("Metadata Results:");
        for (BulkResponse resp : bulk.getResponses()) {
            if (resp.getResult() != Result.OK) {
                System.out.println(
                        "Address: " + resp.getAddress() + " : error:" + resp.getResult());
                continue;
            }
            MetadataT mt = MetadataT.deserializeFromBinary(resp.getData().getFlatbuffers());
            System.out.println("Address: " + resp.getAddress() + " : description: '" + mt.getDescription() + "'");
        }
    }

    private static void bulkRead(Bulk bulk) {
        System.out.println("Read:");
        BulkReadArg[] reqs = new BulkReadArg[] {
                BulkReadArg.create("framework/metrics/system/cpu-utilisation-percent", null),
                BulkReadArg.create("framework/metrics/system/memavailable-mb", null) };

        Result res = bulk.read(reqs);
        if (res != Result.OK) {
            error("bulkRead");
            return;
        }
        System.out.println("Read Results:");
        for (BulkResponse resp : bulk.getResponses()) {
            if (resp.getResult() != Result.OK) {
                System.out.println(
                        "Address: " + resp.getAddress() + " : error:" + resp.getResult());
                continue;
            }
            System.out.println("Address: " + resp.getAddress() + " : value: " + resp.getData().getFLOAT64());
        }
    }

    private static void runAsync(Client c) {
        try (Bulk bulk = c.createBulk()) {

            bulkReadAsync(bulk);
            bulkMetadataAsync(bulk);
            bulkBrowseAsync(bulk);

        } catch (Throwable e) {
            e.printStackTrace();
        }
    }

    private static void bulkBrowseAsync(Bulk bulk) {
        CompletableFuture<BulkResponse[]> futureResponse = new CompletableFuture<>();

        BulkResponseCallback callback;
        callback = (responses) -> {
            futureResponse.complete(responses);
        };

        System.out.println("Async Browse:");
        String[] reqs = new String[] { "framework/metrics/system",
                "framework/metrics/process" };
        Result res = bulk.browse(callback, reqs);
        if (res != Result.OK) {
            error("bulkBrowse");
            return;
        }
        try {
            BulkResponse[] resps = futureResponse.get(5, TimeUnit.SECONDS); // Wait up to 5 seconds
            System.out.println("Async Browse Results:");
            for (BulkResponse resp : resps) {
                if (resp.getResult() != Result.OK) {
                    System.out.println(
                            "Address: " + resp.getAddress() + " : error:" + resp.getResult());
                    continue;
                }
                String commaSeparated = String.join(", ", resp.getData().getArrayOfStrings());
                System.out.println(
                        "Address: " + resp.getAddress() + " : values: '" + commaSeparated + "'");
            }
        } catch (InterruptedException | ExecutionException | TimeoutException e) {
            error("bulkBrowseAsync: exception: " + e);
            return;
        }
    }

    private static void bulkReadAsync(Bulk bulk) {
        CompletableFuture<BulkResponse[]> futureResponse = new CompletableFuture<>();

        BulkResponseCallback callback;
        callback = (responses) -> {
            futureResponse.complete(responses);
        };

        BulkReadArg[] reqs = new BulkReadArg[] {
                BulkReadArg.create("framework/metrics/system/cpu-utilisation-percent", null),
                BulkReadArg.create("framework/metrics/system/memavailable-mb", null) };

        Result res = bulk.read(callback, reqs);
        if (res != Result.OK) {
            error("bulkReadAsync");
            return;
        }

        try {
            BulkResponse[] resps = futureResponse.get(5, TimeUnit.SECONDS); // Wait up to 5 seconds
            System.out.println("Async Read Results:");
            for (BulkResponse resp : resps) {
                // or. for (BulkResponse resp : bulk.getResponses()) {
                if (resp.getResult() != Result.OK) {
                    System.out.println(
                            "Address: " + resp.getAddress() + " : error:" + resp.getResult());
                    continue;
                }
                System.out.println("Address: " + resp.getAddress() + " : value: " + resp.getData().getFLOAT64());
            }
        } catch (InterruptedException | ExecutionException | TimeoutException e) {
            error("bulkReadAsync: exception: " + e);
            return;
        }
    }

    private static void bulkMetadataAsync(Bulk bulk) {
        CompletableFuture<BulkResponse[]> futureResponse = new CompletableFuture<>();

        BulkResponseCallback callback;
        callback = (responses) -> {
            futureResponse.complete(responses);
        };

        System.out.println("Async Metadata:");
        String[] reqs = new String[] { "framework/metrics/system/cpu-utilisation-percent",
                "framework/metrics/system/memavailable-mb" };
        Result res = bulk.metadata(callback, reqs);
        if (res != Result.OK) {
            error("bulkMetadataAsync");
            return;
        }
        try {
            BulkResponse[] resps = futureResponse.get(5, TimeUnit.SECONDS); // Wait up to 5 seconds
            System.out.println("Async Metadata Results:");
            for (BulkResponse resp : resps) {
                if (resp.getResult() != Result.OK) {
                    System.out.println(
                            "Address: " + resp.getAddress() + " : error:" + resp.getResult());
                    continue;
                }
                MetadataT mt = MetadataT.deserializeFromBinary(resp.getData().getFlatbuffers());
                System.out.println("Address: " + resp.getAddress() + " : description: '" + mt.getDescription() + "'");
            }
        } catch (InterruptedException | ExecutionException | TimeoutException e) {
            error("bulkMetadataAsync: exception: " + e);
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
}
