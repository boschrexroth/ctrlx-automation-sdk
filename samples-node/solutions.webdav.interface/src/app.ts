#!/usr/bin/env node
// !!! DO NOT REMOVE THE SHEBANG ON TOP OF THE FILE, WHICH SPECIFIES THIS APP TO BE EXECUTED BY NODE.JS !!!

// Copyright (c) 2023 Bosch Rexroth AG

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the 'Software'), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

import { createClient } from "webdav";

// Disable TLS validation because of self-signed certificate on ctrlX
process.env["NODE_TLS_REJECT_UNAUTHORIZED"] = '0';

// Uncomment this line if proxy should not be used (some proxies do not support WebDAV protocol)
// process.env["HTTPS_PROXY"] = '';

async function webdavClient() {
    console.log('WebDAV-Client for Node.js!');

    // Create client for virtual control
    const client = createClient("https://localhost:8443/solutions/webdav", {
        username: "boschrexroth",   // The user needs 'manage configurations' permissions
        password: "boschrexroth"
    });

    try {
        // Create new directory
        await client.createDirectory('webdavArchive');

        // Create new file
        await client.putFileContents('/webdavArchive/configuration.json', `{"description": "Created with WebDAV-Client"}`);

        // Get file contents as text
        const txt = await client.getFileContents("/webdavArchive/configuration.json", { format: "text" });
        
        // Get file contents binary
        const bin = await client.getFileContents("/webdavArchive/configuration.json");

        // Copy file
        await client.copyFile('/webdavArchive/configuration.json', '/webdavArchive/configuration.json.copy');

        // Get directory contents
        const directoryContents = await client.getDirectoryContents("/webdavArchive");
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

        // Delete file
        await client.deleteFile('/webdavArchive/configuration.json.copy');

        // Delete directory
        await client.deleteFile('webdavArchive');

    } catch(err) {
        console.log(`${err}`);
    }
}

webdavClient();