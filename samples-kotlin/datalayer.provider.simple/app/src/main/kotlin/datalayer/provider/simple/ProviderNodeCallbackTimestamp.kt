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
 * Provider callback for a timestamp node.
 *
 * Reads always return the current wall clock time.
 */
class ProviderNodeCallbackTimestamp : ProviderNodeCallback {

    private val log = System.getLogger(ProviderNodeCallbackTimestamp::class.java.name)

    override fun onBrowse(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onBrowse:")
        ev.callback(Result.OK, null)
    }

    override fun onCreate(ev: ProviderNodeEventData) {
        log.log(System.Logger.Level.INFO, "onCreate:")
    }

    override fun onMetadata(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onMetadata: '${ev.address}'")
        val mask = AllowedOperation.createMask(AllowedOperation.READ)
        val meta = MetadataBuilder.create(mask, "Timestamp Variable", "")
        meta.setNodeClass(NodeClass.Variable)
        meta.addReference(ReferenceType.WRITE, "types/datalayer/timestamp")
        ev.callback(Result.OK, meta.build())
    }

    override fun onRead(ev: ProviderNodeEventData) {
        log.log(System.Logger.Level.INFO, "onRead: '${ev.address}'")
        // Snapshot current time on every read request.
        val variant = Variant.create()
        variant.setInstant(ProviderValues.currentTimestampValue())
        ev.callback(Result.OK, variant)
    }

    override fun onRemove(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onRemove:")
        ev.callback(Result.OK, null)
    }

    override fun onWrite(ev: ProviderNodeEventData) {
        // The sample keeps this node read-only.
        ev.callback(Result.OK, null)
    }
}
