#!/usr/bin/env node
// !!! DO NOT REMOVE THE SHEBANG ON TOP OF THE FILE, WHICH SPECIFIES THIS APP TO BE EXECUTED BY NODE.JS !!!

/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

import { createClient } from "webdav";

// Disable TLS validation because of self-signed certificate on ctrlX
process.env["NODE_TLS_REJECT_UNAUTHORIZED"] = '0';

// Uncomment this line if proxy should not be used (some proxies do not support WebDAV protocol)
// process.env["HTTPS_PROXY"] = '';

// The main function
async function main() {
    console.log('WebDAV-Client for Node.js!');

    // Create client for virtual control
    const client = createClient("https://localhost:8443/solutions/webdav", {
        username: "boschrexroth",   // The user needs 'manage configurations' permissions
        password: "boschrexroth"
    });

    // Create new directory
    await client.createDirectory('webdavArchive');

    // Create new file
    await client.putFileContents('/webdavArchive/configuration.json', `{"description": "Created with WebDAV-Client"}`);

    // Get file contents as text
    const txt = await client.getFileContents("/webdavArchive/configuration.json", { format: "text" });
    console.info(txt);

    // Get file contents binary
    const bin = await client.getFileContents("/webdavArchive/configuration.json");
    console.info(bin);

    // Copy file
    await client.copyFile('/webdavArchive/configuration.json', '/webdavArchive/configuration.json.copy');

    // Get directory contents
    const directoryContents = await client.getDirectoryContents("/webdavArchive");
    console.info(directoryContents);
    // returns a structure like:
    // [
    //   {
    //     filename: "/webdavArchive/configuration.json",
    //     basename: "configuration.json",
    //     lastmod: "Tue, 07 Mar 2023 23:24:11 GMT",
    //     size: 45,
    //     type: "file"
    //   },
    //   {
    //     filename: "/webdavArchive/configuration.json.copy",
    //     basename: "configuration.json.copy",
    //     lastmod: "Tue, 07 Mar 2023 23:24:11 GMT",
    //     size: 45,
    //     type: "file"
    //   },
    //]

    // Handle process event 'SIGTERM'
    process.on('SIGTERM', () => {
        console.info('SIGTERM signal received.');

        // Clean up
        client.deleteFile('/webdavArchive/configuration.json');
        client.deleteFile('/webdavArchive/configuration.json.copy');
        client.deleteFile('webdavArchive');
    });
}

main();