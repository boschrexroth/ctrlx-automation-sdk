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

import DataRange from './datarange';

/**
 * The data ranges of all data types.
 */
const DataRanges = {

    /**
    * bool 8 bit	
    */
    bool8: DataRange.create(false, true),

    /**
     * signed int 8 bit
     */
    int8: DataRange.create(-128, 127),

    /**
     * unsigned int 8 bit
     */
    uint8: DataRange.create(0, 255),

    /**
     * signed int 16 bit
     */
    int16: DataRange.create(-32_768, 32_767),

    /**
     * unsigned int 16 bit
     */
    uint16: DataRange.create(0, 65_535),

    /**
     * signed int 32 bit
     */
    int32: DataRange.create(-2_147_483_648, 2_147_483_647),

    /**
     * unsigned int 32 bit
     */
    uint32: DataRange.create(0, 4_294_967_295),

    /**
     * signed int 64 bit
     */
    int64: DataRange.create(-9_223_372_036_854_775_808n, 9_223_372_036_854_775_807n),

    /**
     * unsigned int 64 bit
     */
    uint64: DataRange.create(0n, 18_446_744_073_709_551_615n),

    /**
     * float 32 bit
     */
    float: DataRange.create(-3.402823e38, 3.402823e38),

    /**
     * float 64 bit
     */
    double: DataRange.create(-1.7976931348623157e+308, 1.7976931348623157e+308),
}

export = DataRanges;