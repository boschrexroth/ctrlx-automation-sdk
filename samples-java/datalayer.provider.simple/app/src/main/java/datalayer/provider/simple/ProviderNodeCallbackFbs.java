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
import pkg.sample.schema.InertialValueT;

public class ProviderNodeCallbackFbs implements ProviderNodeCallback {

    private static final java.lang.System.Logger LOG = java.lang.System.getLogger(ProviderNodeCallbackFbs.class.getName());

    String typeaddressfbs;
    public ProviderNodeCallbackFbs(String addressfbs) {
        typeaddressfbs = addressfbs;
    }

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
        MetadataBuilder meta = MetadataBuilder.create(AllowedOperation.READ.getValue(), "fbs Variable", "");
        meta.setNodeClass(NodeClass.Variable);
        meta.addReference(ReferenceType.READ, typeaddressfbs);
        Variant v = meta.build();
        ev.callback(Result.OK, v);
    }

    @Override
    public void onRead(ProviderNodeEventData ev) {
        LOG.log(Level.INFO, "onRead: '" + ev.getAddress());

        pkg.sample.schema.InertialValueT inertialValue = new InertialValueT();
        inertialValue.setX((short)1);
        inertialValue.setY((short)2);
        inertialValue.setZ((short)3);

        byte [] vals = inertialValue.serializeToBinary();
        Variant val = Variant.create();
        val.setFlatbuffers(vals);
        ev.callback(Result.OK, val);
    }

    @Override
    public void onRemove(ProviderNodeEvent ev) {
        LOG.log(Level.INFO, "onRemove:");
        ev.callback(Result.OK, null);
    }

    @Override
    public void onWrite(ProviderNodeEventData ev) {
        LOG.log(Level.INFO, "onWrite:");
        ev.callback(Result.OK, null);
    }

}
