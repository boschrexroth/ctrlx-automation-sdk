/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

import IClient from 'ctrlx-datalayer/dist/client';

class Browser {

    private client: IClient;
    constructor(client: IClient) {
        this.client = client;
    }

    /**
     * Traverses the ctrlX Data Layer tree and write node with value to console.
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