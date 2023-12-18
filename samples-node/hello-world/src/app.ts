#!/usr/bin/env node
// !!! DO NOT REMOVE THE SHEBANG ON TOP OF THE FILE, WHICH SPECIFIES THIS APP TO BE EXECUTED BY NODE.JS !!!

/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

async function helloWorld() {
    // Keep the process alive until disconnected
    setInterval(() => {
        console.log('Hello World from Node.js!', new Date().toUTCString());
    }, 5_000);

    // Handle process event 'SIGTERM'
    process.on('SIGTERM', () => {
        console.info('SIGTERM signal received.');
    });
}

helloWorld();