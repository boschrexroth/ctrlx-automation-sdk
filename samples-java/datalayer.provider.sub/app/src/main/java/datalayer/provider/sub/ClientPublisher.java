/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package datalayer.provider.sub;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.locks.ReentrantLock;
import java.lang.System.Logger.Level;

import com.boschrexroth.api.NotifyItemPublish;
import com.boschrexroth.api.ProviderSubscription;
import com.boschrexroth.api.Result;

public class ClientPublisher {

    private static final java.lang.System.Logger LOG = java.lang.System.getLogger(ClientPublisher.class.getName());

    int mcounter = 0;
    ProviderSubscription msub;
    Timer mtimer;
    private ReentrantLock mmutex;

    public ClientPublisher(ProviderSubscription subscription, ReentrantLock mutex) {
        msub = subscription;
        mmutex = mutex;
    }

    public void start() {
        LOG.log(Level.INFO, "start: {0}, publish_interval={1}", msub.getUniqueId(),
                msub.getProps().getPublishInterval());
        if (mtimer != null) {
            return;
        }
        mtimer = new Timer("Subscription: " + msub.getUniqueId());
        mtimer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                LOG.log(Level.INFO, "run: {0}, nodes={1}", msub.getUniqueId(), msub.getNodes().length);
                publish();
            }
        }, 0, msub.getProps().getPublishInterval());

    }

    public void stop() {
        if (mtimer == null) {
            return;
        }
        mtimer.cancel();
        mtimer = null;
    }

    private void publish() {
        mmutex.lock();
        ArrayList<String> nodes = new ArrayList<>(Arrays.asList(msub.getNodes()));
        mmutex.unlock();
        ArrayList<NotifyItemPublish> items = new ArrayList<>();
        for(String node : nodes) {
            NotifyItemPublish item = NotifyItemPublish.create(node);
            item.getData().setInt32(mcounter);
            mcounter++;
            items.add(item);
        }
        msub.publish(Result.OK, items.toArray(NotifyItemPublish[]::new));
    }

}
