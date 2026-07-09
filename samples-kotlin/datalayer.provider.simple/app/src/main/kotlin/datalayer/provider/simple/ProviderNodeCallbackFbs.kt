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
 * Provider callback for a node that returns a FlatBuffers payload.
 *
 * The referenced type address is registered by the main application before the node is used.
 */
class ProviderNodeCallbackFbs(private val typeAddressFbs: String) : ProviderNodeCallback {

    private val log = System.getLogger(ProviderNodeCallbackFbs::class.java.name)

    override fun onBrowse(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onBrowse:")
        ev.callback(Result.OK, null)
    }

    override fun onCreate(ev: ProviderNodeEventData) {
        log.log(System.Logger.Level.INFO, "onCreate:")
    }

    override fun onMetadata(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onMetadata: '${ev.address}'")
        val meta = MetadataBuilder.create(AllowedOperation.READ.value, "fbs Variable", "")
        meta.setNodeClass(NodeClass.Variable)
        meta.addReference(ReferenceType.READ, typeAddressFbs)
        ev.callback(Result.OK, meta.build())
    }

    override fun onRead(ev: ProviderNodeEventData) {
        log.log(System.Logger.Level.INFO, "onRead: '${ev.address}'")

        // Build a small synthetic payload encoded with the generated schema.
        val bytes = ProviderValues.flatBuffersBytes()
        val variant = Variant.create()
        variant.setFlatbuffers(bytes)
        ev.callback(Result.OK, variant)
    }

    override fun onRemove(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onRemove:")
        ev.callback(Result.OK, null)
    }

    override fun onWrite(ev: ProviderNodeEventData) {
        // This sample node does not support write operations.
        log.log(System.Logger.Level.INFO, "onWrite:")
        ev.callback(Result.OK, null)
    }
}
