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
 * Provider callback for a read-only string node.
 *
 * Each read cycles through a small list of sample values.
 */
class ProviderNodeCallbackString : ProviderNodeCallback {

    private val log = System.getLogger(ProviderNodeCallbackString::class.java.name)

    override fun onBrowse(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onBrowse:")
        ev.callback(Result.OK, null)
    }

    override fun onCreate(ev: ProviderNodeEventData) {
        log.log(System.Logger.Level.INFO, "onCreate:")
    }

    override fun onMetadata(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onMetadata: '${ev.address}'")
        val meta = MetadataBuilder.create(AllowedOperation.READ.value, "String Variable", "")
        meta.setNodeClass(NodeClass.Variable)
        meta.addReference(ReferenceType.READ, "types/datalayer/string")
        ev.callback(Result.OK, meta.build())
    }

    override fun onRead(ev: ProviderNodeEventData) {
        val value = ProviderValues.nextStringValue()
        log.log(System.Logger.Level.INFO, "onRead: '${ev.address}': $value")
        // Use a variant so Data Layer clients can decode the payload type safely.
        val variant = Variant.create()
        variant.setSTRING(value)
        ev.callback(Result.OK, variant)
    }

    override fun onRemove(ev: ProviderNodeEvent) {
        log.log(System.Logger.Level.INFO, "onRemove:")
        ev.callback(Result.OK, null)
    }

    override fun onWrite(ev: ProviderNodeEventData) {
        // Kept intentionally read-only for this sample.
        log.log(System.Logger.Level.INFO, "onWrite:")
        ev.callback(Result.OK, null)
    }
}
