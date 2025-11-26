/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

 package datalayer.client.sub;

import org.junit.jupiter.api.Test;

import datalayer.client.sub.App;

import static org.junit.jupiter.api.Assertions.*;

class AppTest {
    @Test void appHasAGreeting() {
        App classUnderTest = new App();
        assertTrue(classUnderTest.setupSystem());

    }
}
