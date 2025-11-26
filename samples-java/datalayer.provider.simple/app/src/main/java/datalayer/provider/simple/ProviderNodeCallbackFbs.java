/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package datalayer.provider.simple;

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

    String typeaddressfbs;
    public ProviderNodeCallbackFbs(String addressfbs) {
        typeaddressfbs = addressfbs;
    }

    @Override
    public void onBrowse(ProviderNodeEvent ev) {
        System.out.println("onBrowse:");
        ev.callback(Result.OK, null);
    }

    @Override
    public void onCreate(ProviderNodeEventData ev) {
        System.out.println("onCreate:");
    }

    @Override
    public void onMetadata(ProviderNodeEvent ev) {
        System.out.println("onMetadata: '" + ev.getAddress() + "'");
        MetadataBuilder meta = MetadataBuilder.create(AllowedOperation.READ.getValue(), "fbs Variable", "");
        meta.setNodeClass(NodeClass.Variable);
        meta.addReference(ReferenceType.READ, typeaddressfbs);
        Variant v = meta.build();
        ev.callback(Result.OK, v);
    }

    @Override
    public void onRead(ProviderNodeEventData ev) {
        System.out.println("onRead: '" + ev.getAddress());

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
        System.out.println("onRemove:");
        ev.callback(Result.OK, null);
    }

    @Override
    public void onWrite(ProviderNodeEventData ev) {
        System.out.println("onWrite:");
        ev.callback(Result.OK, null);
    }

}
