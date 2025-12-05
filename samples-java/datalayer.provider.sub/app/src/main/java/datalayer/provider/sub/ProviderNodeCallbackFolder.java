/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package datalayer.provider.sub;

import java.lang.System.Logger.Level;
import java.util.concurrent.locks.ReentrantLock;

import com.boschrexroth.api.AllowedOperation;
import com.boschrexroth.api.MetadataBuilder;
import com.boschrexroth.api.ProviderNodeEvent;
import com.boschrexroth.api.ProviderNodeEventData;
import com.boschrexroth.api.ProviderNodeSubCallback;
import com.boschrexroth.api.ProviderNodeSubscription;
import com.boschrexroth.api.Result;
import com.boschrexroth.api.Variant;

import comm.datalayer.NodeClass;

public class ProviderNodeCallbackFolder implements ProviderNodeSubCallback {

    private static final java.lang.System.Logger LOG = java.lang.System
            .getLogger(ProviderNodeCallbackFolder.class.getName());

    public static final String addressfolderRoot = "sdk/java/provider/sub/";
    public static final String addressfolder = addressfolderRoot + "**";

    private ReentrantLock mutex = new ReentrantLock();

    private final java.util.concurrent.ConcurrentHashMap<Long, ClientPublisher> gSubscriptions = new java.util.concurrent.ConcurrentHashMap<>();

    int _index = 0;

    @Override
    public void onBrowse(ProviderNodeEvent ev) {
        LOG.log(Level.INFO, "onBrowse: {0}", ev.getAddress());
        Variant v = Variant.create();
        v.setArrayOfStrings(new String[] {});
        ev.callback(Result.OK, v);
    }

    @Override
    public void onCreate(ProviderNodeEventData ev) {
        LOG.log(Level.INFO, "onCreate: {0}", ev.getAddress());
        ev.callback(Result.OK, null);
    }

    @Override
    public void onMetadata(ProviderNodeEvent ev) {
        LOG.log(Level.INFO, "onMetadata: {0}", ev.getAddress());
        int mask = AllowedOperation.createMask(AllowedOperation.READ, AllowedOperation.WRITE);
        MetadataBuilder meta = MetadataBuilder.create(mask, "Folder", "");
        meta.setNodeClass(NodeClass.Folder);
        Variant v = meta.build();
        ev.callback(Result.OK, v);
    }

    @Override
    public void onRead(ProviderNodeEventData ev) {
        LOG.log(Level.INFO, "onRead: {0}:{1}", ev.getAddress(), _index);
        Variant val = Variant.create();
        val.setInt32(_index);
        ev.callback(Result.OK, val);
    }

    @Override
    public void onRemove(ProviderNodeEvent ev) {
        LOG.log(Level.INFO, "onRemove: {0}", ev.getAddress());
        ev.callback(Result.UNSUPPORTED, null);
    }

    @Override
    public void onWrite(ProviderNodeEventData ev) {
        LOG.log(Level.INFO, "onWrite: {0}:{1}", ev.getAddress(), _index);
        _index = ev.getData().getInt32();
        ev.callback(Result.OK, ev.getData());
    }

    @Override
    public void onSubscribe(ProviderNodeSubscription sub) {
        LOG.log(Level.INFO, "onSubscribe: {0}:{1}", sub.getAddress(), sub.getSubscription().getUniqueId());

        if (gSubscriptions.containsKey(sub.getSubscription().getUniqueId())) {
            return;
        }

        ClientPublisher cp = new ClientPublisher(sub.getSubscription(), mutex);
        gSubscriptions.put(sub.getSubscription().getUniqueId(), cp);
        cp.start();
    }

    @Override
    public void onUnsubscribe(ProviderNodeSubscription sub) {
        mutex.lock();
        LOG.log(Level.INFO, "onUnsubscribe: {0}:{1}:len={2}", sub.getAddress(), sub.getSubscription().getUniqueId(),
                sub.getSubscription().getNodes().length);
        if (sub.getSubscription().getNodes().length != 0) {
            mutex.unlock();
            return;
        }
        ClientPublisher cp = gSubscriptions.remove(sub.getSubscription().getUniqueId());
        mutex.unlock();
        if (cp == null) {
            return;
        }
        cp.stop();
    }
}
