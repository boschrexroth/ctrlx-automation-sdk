/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package datalayer.provider.simple

import com.boschrexroth.api.AllowedOperation
import com.boschrexroth.api.MetadataBuilder
import com.boschrexroth.api.ProviderNodeCallback
import com.boschrexroth.api.ProviderNodeEvent
import com.boschrexroth.api.ProviderNodeEventData
import com.boschrexroth.api.ReferenceType
import com.boschrexroth.api.Result
import com.boschrexroth.api.Variant
import comm.datalayer.NodeClass

/**
 * Provider callback for an integer node that supports read and write access.
 */
class ProviderNodeCallbackInteger : ProviderNodeCallback {

    private val log = System.getLogger(ProviderNodeCallbackInteger::class.java.name)

    override fun onBrowse(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onBrowse:")
        ev.callback(Result.OK, null)
    }

    override fun onCreate(ev: ProviderNodeEventData) {
        log.log(System.Logger.Level.INFO, "onCreate:")
    }

    override fun onMetadata(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onMetadata: '${ev.address}'")
        // Expose both read and write operations for the Data Layer browser/client.
        val mask = AllowedOperation.createMask(AllowedOperation.READ, AllowedOperation.WRITE)
        val meta = MetadataBuilder.create(mask, "Integer Variable", "")
        meta.setNodeClass(NodeClass.Variable)
        meta.addReference(ReferenceType.READ, "types/datalayer/int32")
        meta.addReference(ReferenceType.WRITE, "types/datalayer/int32")
        ev.callback(Result.OK, meta.build())
    }

    override fun onRead(ev: ProviderNodeEventData) {
        val value = ProviderValues.currentIntegerValue()
        log.log(System.Logger.Level.INFO, "onRead: '${ev.address}': $value")
        // Transport the current integer value as a Data Layer variant.
        val variant = Variant.create()
        variant.setInt32(value)
        ev.callback(Result.OK, variant)
    }

    override fun onRemove(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onRemove:")
        ev.callback(Result.OK, null)
    }

    override fun onWrite(ev: ProviderNodeEventData) {
        val value = ev.data.int32
        log.log(System.Logger.Level.INFO, "onWrite: '${ev.address}': $value")
        // Persist the value in-memory so subsequent reads return the latest write.
        ProviderValues.updateIntegerValue(value)
        ev.callback(Result.OK, ev.data)
    }
}
