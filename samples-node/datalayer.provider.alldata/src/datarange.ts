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
 * The IDataRange interface.
 */
interface IDataRange {
    getMin(): any;
    getMax(): any;
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

    constructor(min: any, max: any) {
        this.min = min;
        this.max = max;
    }

    /**
     * Creates a data range.
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
