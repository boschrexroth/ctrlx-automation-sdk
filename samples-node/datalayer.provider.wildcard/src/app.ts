#!/usr/bin/env node
// !!! DO NOT REMOVE THE SHEBANG ON TOP OF THE FILE, WHICH SPECIFIES THIS APP TO BE EXECUTED BY NODE.JS !!!

/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

import DatalayerSystem from 'ctrlx-datalayer/dist/datalayersystem';
import Result from 'ctrlx-datalayer/dist/result';
import ProviderNodeCallbacks from 'ctrlx-datalayer/dist/providernodecallbacks';
import IProviderNodeResult from 'ctrlx-datalayer/dist/providernoderesult';
import IVariant from 'ctrlx-datalayer/dist/variant';
import VariantTypes from 'ctrlx-datalayer/dist/varianttypes';

// Utils
import * as MetadataUtils from 'ctrlx-datalayer/dist/metadata-utils';
import { Remote } from 'ctrlx-datalayer/dist/remote';

// The node type interface.
interface Node {
    address: string;
    value: any;
    metadata: Uint8Array;
}

// The nodes.
let nodes = {};

async function main() {

    const rootAddress = 'sdk/node/provider/wildcard';

    // Create some  nodes here just for demonstration.
    console.log(`creating virtual nodes on root address: ${rootAddress}`);
    createRootNode(rootAddress);
    createDummyNodes(rootAddress, 3, 5);

    // Create a new ctrlX Data Layer system.
    const system = new DatalayerSystem('');

    // Start the ctrlX Data Layer system without a new broker (startBroker = false), because one broker is already running on ctrlX CORE.
    await system.start(false);

    // Create a remote address with the parameters according to your environment.
    const remote = Remote.build({ ip: '192.168.1.1', sslPort: 443 });
    console.log('connection string:', remote);

    // Create a Datalayer Provider instance and connect. Automatically reconnects if the connection is interrupted.
    const provider = await system.createProvider(remote);
    await provider.start();

    // Initially exit and retry after app restart-delay (see snapcraft.yaml).
    if (provider.isConnected() === false) {
        console.log('provider is not connected -> exit.');
        return;
    }

    // Implement onBrowse callback function.
    const onBrowse = (address: string, result: IProviderNodeResult) => {
        console.log('onBrowse:', address);

        // We return the list of child node names of the current branch, not leaf nodes which hold a value itself.

        // Split given address into parts.
        let addressParts = address.split('/');

        // Exclude leaf nodes and filter for current level node names.
        const keys = Object.keys(nodes);
        const branches = keys.filter(value => value.length > address.length && value.startsWith(`${address}/`));

        // Collect child level names, preventing multiples.
        let branchLevelNames: string[] = []
        for (let branch of branches) {
            const parts = branch.split('/');

            let branchLevelName: string;
            if (parts.length > addressParts.length) {
                branchLevelName = parts[addressParts.length];
            } else {
                branchLevelName = parts[parts.length - 1];
            }

            if (!branchLevelNames.includes(branchLevelName)) {
                branchLevelNames.push(branchLevelName);
            }
        }

        // We return a string array with child node names and have to explicitly give the type 'arstring', 
        // because without, empty arrays are not recognizable as an string array and result in DL_INVALID_VALUE.
        result.setResult(Result.OK, branchLevelNames, VariantTypes.arstring);
    };

    // Implement onMetadata callback function.
    const onMetadata = (address: string, result: IProviderNodeResult) => {
        console.log('onMetadata:', address);

        const node: Node = nodes[address];
        if (node !== undefined) {
            // Metadata has to be returned as an Uint8Array with explicitly type 'flatbuffers'.
            result.setResult(Result.OK, node.metadata, VariantTypes.flatbuffers);
            return;
        }
        result.setResult(Result.UNSUPPORTED, null);
    };

    // Implement onRead callback function.
    const onRead = (address: string, args: IVariant, result: IProviderNodeResult) => {
        console.log('onRead:', address);

        const node: Node = nodes[address];
        if (node !== undefined) {
            result.setResult(Result.OK, node.value);
            return;
        }
        result.setResult(Result.FAILED, null);
    };

    // Implement onWrite callback function.
    const onWrite = (address: string, args: IVariant, result: IProviderNodeResult) => {
        console.log('onWrite:', address);

        const node: Node = nodes[address];
        if (node !== undefined) {
            node.value = args.value;
            result.setResult(Result.OK, node.value);
            return;
        }
        result.setResult(Result.UNSUPPORTED, null)
    };

    // Configure callbacks we handle.
    const providerNodeCallbacks = new ProviderNodeCallbacks()
        .setOnMetadata(onMetadata)
        .setOnBrowse(onBrowse)
        .setOnRead(onRead)
        .setOnWrite(onWrite);

    // Register the root node by appending the wildcard '/*' to just support one level of sub nodes, else '/**' for any unlimited hierarchy.
    await provider.registerNode(`${rootAddress}/**`, providerNodeCallbacks).then((result) => console.log('registered node', rootAddress, 'with result:', result));

    // Keep the process alive until disconnected.
    const intervalHandle = setInterval(() => {

        if (system.isStarted() === false || provider.isConnected() === false) {
            clearInterval(intervalHandle);
        }
    }, 1_000);

    // Handle process event 'SIGTERM'.
    process.on('SIGTERM', () => {
        console.info('SIGTERM signal received.');

        // Stop the provider.
        provider.stop();
        console.log('Provider stopped.');

        system.stop();
        console.log('ctrlX Data Layer system stopped.');
    });
}

// Creates and adds dummy nodes with random strings.
let currentLevel = 0;
function createDummyNodes(currentPath: string, nodesPerLevel: number, maxDepth: number) {
    const nameLength = 5;
    currentLevel++;

    for (let i = 0; i < nodesPerLevel; i++) {
        const address = `${currentPath}/${randomString(nameLength)}`;

        // Metadata for child nodes: Variable (int32)

        // Please see here for recommended allowed operations by node type: 
        // https://docs.automation.boschrexroth.com/doc/1925281162/metadata/latest/en/ 

        const node: Node = {
            address: address,
            value: address,
            metadata: new MetadataUtils.MetadataBuilder(
                MetadataUtils.AllowedOperationFlags.Read | MetadataUtils.AllowedOperationFlags.Write | MetadataUtils.AllowedOperationFlags.Browse, '', '')
                .setNodeClass(MetadataUtils.NodeClass.Variable)
                .addReference(MetadataUtils.ReferenceType.ReadType, 'types/datalayer/int32')
                .addReference(MetadataUtils.ReferenceType.WriteType, 'types/datalayer/int32')
                .build()
        };

        if (currentLevel < maxDepth) {
            createDummyNodes(address, nodesPerLevel, maxDepth);
        }

        nodes[address] = node;
    }
}

// Creates and adds our root node (without wildcards).
function createRootNode(address: string) {

    // Metadata for root node (Folder).

    // Please see here for recommended allowed operations by node type: 
    // https://docs.automation.boschrexroth.com/doc/1925281162/metadata/latest/en/ 

    const rootNode: Node = {
        address: address,
        value: address,
        metadata: new MetadataUtils.MetadataBuilder(
            MetadataUtils.AllowedOperationFlags.Browse, address, '')
            .setNodeClass(MetadataUtils.NodeClass.Folder)
            .build()
    };

    nodes[address] = rootNode;
}

// Creates a random string with given length.
function randomString(length: number) {
    const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
    let result = '';
    for (let i = 0; i < length; i++) {
        const randomIndex = Math.floor(Math.random() * characters.length);
        result += characters[randomIndex];
    }
    return result;
}

main();