// Copyright (c) 2021-2022 Bosch Rexroth AG

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

// MetadataUtils
import * as MetadataUtils from 'ctrlx-datalayer/dist/metadata-utils';
import DatalayerSystem from 'ctrlx-datalayer/dist/datalayersystem';

// Flatbuffers
import * as flatbuffers from 'flatbuffers';
import { InertialValue } from './sample/schema/inertial-value';

import DataType from './datatype';
import DataTypes from './datatypes';

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

            case DataTypes.arbool8:
                for (var i = 0; i < this.value.length; ++i) {
                    this.value[i] = !this.value[i];
                }
                break;

            case DataTypes.arint8:
            case DataTypes.aruint8:
            case DataTypes.arint16:
            case DataTypes.aruint16:
            case DataTypes.arint32:
            case DataTypes.aruint32:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.value[i] + 1;
                }
                break;

            case DataTypes.arfloat:
            case DataTypes.ardouble:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.value[i] + 0.1;
                }
                break;

            case DataTypes.arint64:
            case DataTypes.aruint64:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.value[i] + 1n;
                }
                break;

            case DataTypes.arstring:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = this.incrementStringValue(this.value[i], '_');
                }
                break;

            case DataTypes.artimestamp:
                for (let i = 0; i < this.value.length; ++i) {
                    this.value[i] = now;
                }
                break;

            case DataTypes.inertialValue:
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
        var separatorIndex = text.indexOf(separator);
        var prefix = text.substring(0, separatorIndex);
        var postfix = text.substring(separatorIndex + 1);
        var number = Number.parseInt(postfix);
        return `${prefix}${separator}${++number}`;
    }
}

export = Node
