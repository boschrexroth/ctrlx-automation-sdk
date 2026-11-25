#!/usr/bin/env node
// THIS SHEBANG ON TOP OF THE FILE INDICATES THE APP TO BE EXECUTED BY NODE.JS. PLEASE DO NOT REMOVE. 

/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

async function helloWorld() {
    // Keep the process alive until disconnected
    const intervalHandle = setInterval(() => {
        console.log('Hello World from Node.js!', new Date().toUTCString());
    }, 5_000);

    // Common shutdown routine used by both 'SIGTERM' and 'SIGINT'
    const shutdown = (signal: string) => {
        console.info(`${signal} signal received.`);

        // Stop keeping the process alive
        clearInterval(intervalHandle);
    };

    // Handle process events 'SIGTERM' and 'SIGINT'
    process.on('SIGTERM', () => shutdown('SIGTERM'));
    process.on('SIGINT', () => shutdown('SIGINT'));
}

helloWorld();