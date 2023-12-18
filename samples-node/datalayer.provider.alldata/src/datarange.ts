/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * The IDataRange interface.
 */
interface IDataRange {
    getMin(): any
    getMax(): any
}

/**
 * The DataRange class.
 */
class DataRange implements IDataRange {

    /**
     * The min value.
     */
    private min: any;

    /** 
     * The max value.
     */
    private max: any;

    /** 
     * Creates a new instance of the DataRange class
     */
    constructor(min: any, max: any) {
        this.min = min;
        this.max = max;
    }

    /**
     * Factory to create a new instance of the DataRange class.
     * @param min 
     * @param max 
     * @returns 
     */
    static create(min: any, max: any): DataRange {
        return new DataRange(min, max);
    }

    /**
     * Gets the min value.
     * @returns 
     */
    getMin(): any {
        return this.min;
    }

    /**
     * Gets the max value.
     */
    getMax(): any {
        return this.max;
    }
}

export = DataRange
