/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

// Flatbuffers
import * as flatbuffers from 'flatbuffers';
import { InertialValue } from './sampleSchema_generated';

import DataType from './datatype';
import DataTypes from './datatypes';
import DatalayerSystem from 'ctrlx-datalayer/dist/datalayersystem';

// Utils
import * as MetadataUtils from 'ctrlx-datalayer/dist/metadata-utils';

/**
 * The interface INode
 */
interface INode {
    /**
     * Gets the name.
     */
    getName(): string

    /**
     * Gets the address.
     */
    getAddress(): string

    /**
     * Gets the value.
     */
    getValue(): any

    /**
     * Gets the data type.
     */
    getDataType(): DataType

    /**
     * Gets the meta data.
     */
    getMetadata(): Uint8Array
}

/**
 * The class Node.
 */
class Node implements INode {

    // Root node for all nodes
    private static rootNode = 'sdk/node/provider/all-data';

    private address: string;
    private value: any;
    private dataType: DataType;
    private metadata: Uint8Array;

    /** 
     * Creates a new instance of Node class
     */
    constructor(dataType: DataType, address: string, value: any, metadata: Uint8Array) {
        this.dataType = dataType;
        this.address = address;
        this.value = value;
        this.metadata = metadata;
    }

    /**
     * Creates a static node.
     * @param dataType 
     * @param value 
     * @returns The node
     */
    public static createStatic(dataType: DataType, value: any): Node {
        const address = this.rootNode + "/static/" + dataType.getName();
        const allowedOperationFlags = MetadataUtils.AllowedOperationFlags.Read | MetadataUtils.AllowedOperationFlags.Write;
        const description = `Static data with type ${dataType.getName()}`;
        const metadata = new MetadataUtils.MetadataBuilder(allowedOperationFlags, description)
            .setDisplayName(dataType.getName())
            .setNodeClass(MetadataUtils.NodeClass.Variable)
            .addReference(MetadataUtils.ReferenceType.ReadType, dataType.getTargetAddress())
            .addReference(MetadataUtils.ReferenceType.WriteType, dataType.getTargetAddress())
            .build();

        return new Node(
            dataType,
            address,
            value,
            metadata);
    }

    /**
     * Creates a dynamic node.
     * @param dataType 
     * @param value 
     * @returns The node
     */
    public static createDynamic(dataType: DataType, value: any): Node {
        const address = this.rootNode + "/dynamic/" + dataType.getName();
        const allowedOperationFlags = MetadataUtils.AllowedOperationFlags.Read;
        const description = `Dynamic data with type ${dataType.getName()}`;
        const metadata = new MetadataUtils.MetadataBuilder(allowedOperationFlags, description)
            .setDisplayName(dataType.getName())
            .setNodeClass(MetadataUtils.NodeClass.Variable)
            .addReference(MetadataUtils.ReferenceType.ReadType, dataType.getTargetAddress())
            .addReference(MetadataUtils.ReferenceType.WriteType, dataType.getTargetAddress())
            .build();

        return new Node(
            dataType,
            address,
            value,
            metadata);
    }

    /**
     * Gets the address
     * @returns 
     */
    public getAddress(): string {
        return this.address;
    }

    /**
     * Gets the name
     * @returns 
     */
    public getName(): string {
        return this.dataType.getName();
    }

    /**
     * Gets the datatype
     * @returns 
     */
    public getDataType(): DataType {
        return this.dataType;
    }

    /**
     * Gets the value
     * @returns 
     */
    public getValue(): any {
        return this.value;
    }

    /**
     * Sets the value
     * @returns 
     */
    setValue(value: any) {
        this.value = value;
    }

    /**
     * Gets the metadata
     * @returns 
     */
    public getMetadata(): Uint8Array {
        return this.metadata;
    }

    /**
     * Increments the value
     * @returns 
     */
    public incrementValue(now: Date) {
        switch (this.dataType) {
            case DataTypes.bool8:
                this.value = !this.value;
                break;
            case DataTypes.int8:
            case DataTypes.uint8:
            case DataTypes.int16:
            case DataTypes.uint16:
            case DataTypes.int32:
            case DataTypes.uint32:
                this.value += 1;
                break;
            case DataTypes.int64:
            case DataTypes.uint64:
                this.value += 1n;
                break;
            case DataTypes.float:
            case DataTypes.double:
                this.value += 0.1;
                break;
            case DataTypes.string:
                this.value = this.incrementStringValue(this.value, '_');
                break;
            case DataTypes.timestamp:
                this.value = now;
                break;
            case DataTypes.arrayOfBool8:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = !this.value[i];
                }
                break;
            case DataTypes.arrayOfInt8:
            case DataTypes.arrayOfUint8:
            case DataTypes.arrayOfInt16:
            case DataTypes.arrayOfUint16:
            case DataTypes.arrayOfInt32:
            case DataTypes.arrayOfUint32:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.value[i] + 1;
                }
                break;
            case DataTypes.arrayOfFloat:
            case DataTypes.arrayOfFloat64:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.value[i] + 0.1;
                }
                break;
            case DataTypes.arrayOfInt64:
            case DataTypes.arrayOfUint64:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.value[i] + 1n;
                }
                break;
            case DataTypes.arrayOfString:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.incrementStringValue(this.value[i], '_');
                }
                break;
            case DataTypes.arrayOfTimestamp:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = now;
                }
                break;
            case DataTypes.inertialValue:
                {
                    const builder = new flatbuffers.Builder(DatalayerSystem.defaultFlatbuffersInitialSize);
                    const inertialValue = InertialValue.getRootAsInertialValue(new flatbuffers.ByteBuffer(this.value)).unpack();
                    inertialValue.x += 1;
                    inertialValue.y += 1;
                    inertialValue.z += 1;
                    const offset = inertialValue.pack(builder);
                    builder.finish(offset);
                    this.value = builder.asUint8Array();
                }
                break;
        }
    }

    /**
     * Increments a string value
     * @returns 
     */
    private incrementStringValue(text: string, separator: string): string {
        const separatorIndex = text.indexOf(separator);
        const prefix = text.substring(0, separatorIndex);
        const postfix = text.substring(separatorIndex + 1);
        let number = Number.parseInt(postfix);
        return `${prefix}${separator}${++number}`;
    }
}

export = Node
