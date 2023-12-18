/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

import DataRange from './datarange';

/**
 * The data ranges of all data types.
 */
const DATA_RANGES = {

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
};

export = DATA_RANGES;