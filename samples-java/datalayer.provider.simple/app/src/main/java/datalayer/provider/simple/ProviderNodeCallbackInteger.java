/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package datalayer.provider.simple;

import java.lang.System.Logger.Level;

import com.boschrexroth.api.AllowedOperation;
import com.boschrexroth.api.MetadataBuilder;
import com.boschrexroth.api.ProviderNodeCallback;
import com.boschrexroth.api.ProviderNodeEvent;
import com.boschrexroth.api.ProviderNodeEventData;
import com.boschrexroth.api.ReferenceType;
import com.boschrexroth.api.Result;
import com.boschrexroth.api.Variant;

import comm.datalayer.NodeClass;

public class ProviderNodeCallbackInteger implements ProviderNodeCallback {

    private static final java.lang.System.Logger LOG = java.lang.System.getLogger(ProviderNodeCallbackInteger.class.getName());

    int _index = 0;

    @Override
    public void onBrowse(ProviderNodeEvent ev) {
        LOG.log(Level.INFO, "onBrowse:");
        ev.callback(Result.OK, null);
    }

    @Override
    public void onCreate(ProviderNodeEventData ev) {
        LOG.log(Level.INFO, "onCreate:");
    }

    @Override
    public void onMetadata(ProviderNodeEvent ev) {
        LOG.log(Level.INFO, "onMetadata: '" + ev.getAddress() + "'");
        int mask = AllowedOperation.createMask(AllowedOperation.READ, AllowedOperation.WRITE);
        MetadataBuilder meta = MetadataBuilder.create(mask, "Integer Variable", "");
        meta.setNodeClass(NodeClass.Variable);
        meta.addReference(ReferenceType.READ, "types/datalayer/int32");
        meta.addReference(ReferenceType.WRITE, "types/datalayer/int32");
        Variant v = meta.build();
        ev.callback(Result.OK, v);
    }

    @Override
    public void onRead(ProviderNodeEventData ev) {
        LOG.log(Level.INFO, "onRead: '" + ev.getAddress() + "': " + _index);
        Variant val = Variant.create();
        val.setInt32(_index);
        ev.callback(Result.OK, val);
    }

    @Override
    public void onRemove(ProviderNodeEvent ev) {
        LOG.log(Level.INFO, "onRemove:");
        ev.callback(Result.OK, null);
    }

    @Override
    public void onWrite(ProviderNodeEventData ev) {
        LOG.log(Level.INFO, "onWrite: '" + ev.getAddress() + "': " + _index);
        _index = ev.getData().getInt32();
        ev.callback(Result.OK, ev.getData());
    }

}
