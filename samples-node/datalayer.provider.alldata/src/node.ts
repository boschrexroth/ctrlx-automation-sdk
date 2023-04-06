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

// DatalayerSystem
import DatalayerSystem from 'ctrlx-datalayer/dist/datalayersystem';

// Flatbuffers
import * as flatbuffers from 'flatbuffers';
import { InertialValue } from './sample/schema/inertial-value';

import DataType from './datatype';
import DATA_TYPES from './datatypes';
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
    getName(): string;

    /**
     * Gets the address.
     */
    getAddress(): string;

    /**
     * Gets the value.
     */
    getValue(): any;

    /**
     * Gets the data type.
     */
    getDataType(): DataType;

    /**
     * Gets the meta data.
     */
    getMetadata(): Uint8Array;
}

/**
 * The class Node.
 */
class Node implements INode {

    // Root node for all static nodes.
    private static staticRoot = 'samples/nodejs/alldata/static';
    // Root node for all dynamic nodes.
    private static dynamicRoot = 'samples/nodejs/alldata/dynamic'

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
        const address = this.staticRoot + "/" + dataType.getName();
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
        const address = this.dynamicRoot + "/" + dataType.getName();
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
            case DATA_TYPES.bool8:
                this.value = !this.value;
                break;

            case DATA_TYPES.int8:
            case DATA_TYPES.uint8:
            case DATA_TYPES.int16:
            case DATA_TYPES.uint16:
            case DATA_TYPES.int32:
            case DATA_TYPES.uint32:
                this.value += 1;
                break;

            case DATA_TYPES.int64:
            case DATA_TYPES.uint64:
                this.value += 1n;
                break;

            case DATA_TYPES.float:
            case DATA_TYPES.double:
                this.value += 0.1;
                break;

            case DATA_TYPES.string:
                this.value = this.incrementStringValue(this.value, '_');
                break;

            case DATA_TYPES.timestamp:
                this.value = now;
                break;

            case DATA_TYPES.arbool8:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = !this.value[i];
                }
                break;

            case DATA_TYPES.arint8:
            case DATA_TYPES.aruint8:
            case DATA_TYPES.arint16:
            case DATA_TYPES.aruint16:
            case DATA_TYPES.arint32:
            case DATA_TYPES.aruint32:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.value[i] + 1;
                }
                break;

            case DATA_TYPES.arfloat:
            case DATA_TYPES.ardouble:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.value[i] + 0.1;
                }
                break;

            case DATA_TYPES.arint64:
            case DATA_TYPES.aruint64:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.value[i] + 1n;
                }
                break;

            case DATA_TYPES.arstring:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.incrementStringValue(this.value[i], '_');
                }
                break;

            case DATA_TYPES.artimestamp:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = now;
                }
                break;

            case DATA_TYPES.inertialValue:
                const oldInertialValue = InertialValue.getRootAsInertialValue(new flatbuffers.ByteBuffer(this.value))
                const builder = new flatbuffers.Builder(DatalayerSystem.defaultFlatbuffersInitialSize);
                const offset = InertialValue.createInertialValue(builder,
                    oldInertialValue.x() + 1,
                    oldInertialValue.y() + 1,
                    oldInertialValue.z() + 1);
                builder.finish(offset);
                this.value = builder.asUint8Array();
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
