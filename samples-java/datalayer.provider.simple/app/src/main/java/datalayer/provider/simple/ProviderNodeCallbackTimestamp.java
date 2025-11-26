/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package datalayer.provider.simple;

import java.time.Instant;

import com.boschrexroth.api.AllowedOperation;
import com.boschrexroth.api.MetadataBuilder;
import com.boschrexroth.api.ProviderNodeCallback;
import com.boschrexroth.api.ProviderNodeEvent;
import com.boschrexroth.api.ProviderNodeEventData;
import com.boschrexroth.api.ReferenceType;
import com.boschrexroth.api.Result;
import com.boschrexroth.api.Variant;

import comm.datalayer.NodeClass;

public class ProviderNodeCallbackTimestamp implements ProviderNodeCallback {

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
        int mask = AllowedOperation.createMask(AllowedOperation.READ);
        MetadataBuilder meta = MetadataBuilder.create(mask, "Timestamp Variable", "");
        meta.setNodeClass(NodeClass.Variable);
        meta.addReference(ReferenceType.WRITE, "types/datalayer/timestamp");
        Variant v = meta.build();
        ev.callback(Result.OK, v);
    }

    @Override
    public void onRead(ProviderNodeEventData ev) {
        System.out.println("onRead: '" + ev.getAddress());
        Instant time = Instant.now();
        Variant val = Variant.create();
        val.setInstant(time);
        ev.callback(Result.OK, val);
    }

    @Override
    public void onRemove(ProviderNodeEvent ev) {
        System.out.println("onRemove:");
        ev.callback(Result.OK, null);
    }

    @Override
    public void onWrite(ProviderNodeEventData ev) {
        ev.callback(Result.OK, null);
    }

}
