// Copyright (c) 2021-2023 Bosch Rexroth AG

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

import IClient from 'ctrlx-datalayer/dist/client';

class Browser {

    private client: IClient;
    constructor(client: IClient) {
        this.client = client;
    }

    /**
     * Traverses the Data Layer tree and write node with value to console.
     * @param address 
     * @param leaf 
     * @param indent 
     * @param last 
     * @returns 
     */
    public async traverse(address: string = '', leaf: string = '', indent: string = '', last: boolean = true) {
        const prefix = last ? '\\-' : '|-';
        const json = await this.readJson(address);
        console.log(`${indent}${prefix}[${leaf}] ${json}`);
        indent = last ? indent += '  ' : indent += '| ';

        try {
            const variant = await this.client.browse(address);
            const children = variant.value as Array<string>;
            for (let i = 0; i < children.length; i++) {
                const childAddress = address === '' ? children[i] : `${address}/${children[i]}`;
                await this.traverse(childAddress, children[i], indent, i === children.length - 1);
            }
        } catch (error) {
            // do nothing in case of error
        }
    }


    /**
     * Reads the json value.
     * @param address 
     * @returns 
     */
    private async readJson(address: string): Promise<string> {
        if (address === '') {
            return '';
        }

        try {
            const variant = await this.client.readJson(address, -1);
            if (variant != null) {
                const json = variant.value;
                return this.truncate(json, 100);
            }
        } catch (error) {
            return ''
        }

        return ''
    }

    /**
     * Truncates json value to string with max length.
     * @param value 
     * @param maxChars 
     * @returns 
     */
    private truncate(value: string, maxChars: number): string {
        return value.length <= maxChars ? value : value.substring(0, maxChars) + '...';
    }
}

export = Browser;