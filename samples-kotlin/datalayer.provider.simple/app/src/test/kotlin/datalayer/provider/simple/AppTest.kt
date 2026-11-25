/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package datalayer.provider.simple

import org.junit.jupiter.api.Assertions.assertNotNull
import org.junit.jupiter.api.Test

class AppTest {
    @Test
    fun appCanBeInstantiated() {
        val callbackString = ProviderNodeCallbackString()
        assertNotNull(callbackString)
    }
}
