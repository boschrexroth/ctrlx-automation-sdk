#!/usr/bin/env node
// THIS SHEBANG ON TOP OF THE FILE INDICATES THE APP TO BE EXECUTED BY NODE.JS. PLEASE DO NOT REMOVE. 

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