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

public class ProviderNodeCallbackString implements ProviderNodeCallback {

    final static String[] _message = new String[] { "ctrlX", "Software", "Development", "Kit" };
    int _index = 0;

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
        MetadataBuilder meta = MetadataBuilder.create(AllowedOperation.READ.getValue(), "String Variable", "");
        meta.setNodeClass(NodeClass.Variable);
        meta.addReference(ReferenceType.READ, "types/datalayer/string");
        Variant v = meta.build();
        ev.callback(Result.OK, v);
    }

    @Override
    public void onRead(ProviderNodeEventData ev) {
        System.out.println("onRead: '" + ev.getAddress() + "': " + _index);
        if (_index >= _message.length) {
            _index = 0;
        }
        Variant val = Variant.create();
        val.setSTRING(_message[_index]);
        _index++;
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
