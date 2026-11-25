/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

package datalayer.provider.simple

import com.google.flatbuffers.FlatBufferBuilder
import pkg.sample.schema.InertialValue
import java.time.Instant
import java.util.concurrent.atomic.AtomicInteger

/**
 * Central in-memory values used by all provider callback nodes.
 */
object ProviderValues {
    private val stringValues = arrayOf("ctrlX", "Software", "Development", "Kit")
    private val stringIndex = AtomicInteger(0)
    private val integerValue = AtomicInteger(0)

    /** Returns the current string and advances to the next element in a cyclic sequence. */
    fun nextStringValue(): String {
        val currentIndex = stringIndex.getAndUpdate { index -> (index + 1) % stringValues.size }
        return stringValues[currentIndex]
    }

    /** Returns the current string value without advancing the sequence index. */
    fun peekStringValue(): String {
        return stringValues[stringIndex.get()]
    }

    /** Returns the integer value last written by a Data Layer client. */
    fun currentIntegerValue(): Int {
        return integerValue.get()
    }

    /** Updates the in-memory integer value served by the integer provider node. */
    fun updateIntegerValue(value: Int) {
        integerValue.set(value)
    }

    /** Returns the current timestamp used for read operations. */
    fun currentTimestampValue(): Instant {
        return Instant.now()
    }

    /**
     * Generates a tiny FlatBuffers payload matching the sample inertial value schema.
     */
    fun flatBuffersBytes(): ByteArray {
        val builder = FlatBufferBuilder(0)
        val offset = InertialValue.createInertialValue(builder, 1.toShort(), 2.toShort(), 3.toShort())
        InertialValue.finishInertialValueBuffer(builder, offset)
        return builder.sizedByteArray()
    }
}