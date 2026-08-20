/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */



/**
 * The interface IDataType.
 */
interface IDataType {
    /**
     * Gets the name.
     */
    getName(): string

    /**
     * Gets the variant type.
     */
    getVariantType(): string

    /**
     * Gets the target address.
     */
    getTargetAddress(): string

    /**
     * Gets the data type as string.
     */
    toString(): string
}

/**
 * The class DataType.
 */
class DataType implements IDataType {

    static rootType = 'types/sdk/node/provider/all-data';

    /** 
     * Creates a new instance of the DataType class
     */
    constructor(
        private name: string,
        private variantType: string,
        private targetAddress: string
    ) { }

    /**
     * Factory to create a new instance of the DataType class.
     * @param name 
     * @param variantType 
     * @param targetAddress 
     * @returns 
     */
    static create(name: string, variantType: string, targetAddress: string): DataType {
        return new DataType(name, variantType, targetAddress);
    }

    /**
    * Gets the name.
    * @returns 
    */
    getName(): string {
        return this.name;
    }

    /**
    * Gets the variant type.
    * @returns 
    */
    getVariantType(): string {
        return this.variantType;
    }

    /**
    * Gets target address.
    * @returns 
    */
    getTargetAddress(): string {
        return this.targetAddress;
    }

    /**
    * Stringifies the DataType class
    * @returns 
    */
    toString(): string {
        return this.name;
    }
}

export = DataType
