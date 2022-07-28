// Copyright (c) 2021 Bosch Rexroth AG
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

/**
 * The interface IDataType.
 */
interface IDataType {
    /**
     * Gets the name.
     */
    getName(): string;

    /**
     * Gets the variant type.
     */
    getVariantType(): string;

    /**
     * Gets the target address.
     */
    getTargetAddress(): string;

    /**
     * Gets the data type as string.
     */
    toString(): string;
}

/**
 * The class DataType.
 */
class DataType implements IDataType {

    /**
     * The name of the data type.
     */
    private name: string;

    /**
     * The variant type.
     */
    private variantType: string;

    /**
     * The target address.
     */
    private targetAddress: string;

    /** 
     * Creates a new instance of the DataType class
     */
    constructor(name: string, variantType: string, targetAddress: string) {
        this.name = name;
        this.variantType = variantType;
        this.targetAddress = targetAddress;
    }

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
