/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

import VariantTypes from 'ctrlx-datalayer/dist/varianttypes';

import DataType from './datatype';

/**
 * The data types with variant type and target address.
 */
const DATA_TYPES = {
    
    /**
     * bool 8 bit	
     */
    bool8: DataType.create('bool8', VariantTypes.bool8, 'types/datalayer/bool8'),

    /**
     * signed int 8 bit
     */
    int8: DataType.create('int8', VariantTypes.int8, 'types/datalayer/int8'),

    /**
     * unsigned int 8 bit
     */
    uint8: DataType.create('uint8', VariantTypes.uint8, 'types/datalayer/uint8'),

    /**
     * signed int 16 bit
     */
    int16: DataType.create('int16', VariantTypes.int16, 'types/datalayer/int16'),

    /**
     * unsigned int 16 bit
     */
    uint16: DataType.create('uint16', VariantTypes.uint16, 'types/datalayer/uint16'),

    /**
     * signed int 32 bit
     */
    int32: DataType.create('int32', VariantTypes.int32, 'types/datalayer/int32'),

    /**
     * unsigned int 32 bit
     */
    uint32: DataType.create('uint32', VariantTypes.uint32, 'types/datalayer/uint32'),

    /**
     * signed int 64 bit
     */
    int64: DataType.create('int64', VariantTypes.int64, 'types/datalayer/int64'),

    /**
     * unsigned int 64 bit
     */
    uint64: DataType.create('uint64', VariantTypes.uint64, 'types/datalayer/uint64'),

    /**
     * float 32 bit
     */
    float: DataType.create('float32', VariantTypes.float, 'types/datalayer/float32'),

    /**
     * float 64 bit
     */
    double: DataType.create('float64', VariantTypes.double, 'types/datalayer/float64'),

    /**
     * string (UTF-8)
     */
    string: DataType.create('string', VariantTypes.string, 'types/datalayer/string'),

    /**
     * timestamp (Date)
     */
    timestamp: DataType.create('timestamp', VariantTypes.timestamp, 'types/datalayer/timestamp'),

    /**
     * flatbuffers
     */
    inertialValue: DataType.create('inertial-value', VariantTypes.flatbuffers, `${DataType.rootType}/inertial-value`),

    /**
     * array of bool 8 bit
     */
    arrayOfBool8: DataType.create('array-of-bool8', VariantTypes.arbool8, 'types/datalayer/array-of-bool8'),

    /**
     * array of signed int 8 bit
     */
    arrayOfInt8: DataType.create('array-of-int8', VariantTypes.arint8, 'types/datalayer/array-of-int8'),

    /**
     * array of unsigned int 8 bit
     */
    arrayOfUint8: DataType.create('array-of-uint8', VariantTypes.aruint8, 'types/datalayer/array-of-uint8'),

    /**
     * array of signed int 16 bit
     */
    arrayOfInt16: DataType.create('array-of-int16', VariantTypes.arint16, 'types/datalayer/array-of-int16'),

    /**
     * array of unsigned int 16 bit
     */
    arrayOfUint16: DataType.create('array-of-uint16', VariantTypes.aruint16, 'types/datalayer/array-of-uint16'),

    /**
     * array of signed int 32 bit
     */
    arrayOfInt32: DataType.create('array-of-int32', VariantTypes.arint32, 'types/datalayer/array-of-int32'),

    /**
     * array of unsigned int 32 bit
     */
    arrayOfUint32: DataType.create('array-of-uint32', VariantTypes.aruint32, 'types/datalayer/array-of-uint32'),

    /**
     * array of signed int 64 bit
     */
    arrayOfInt64: DataType.create('array-of-int64', VariantTypes.arint64, 'types/datalayer/array-of-int64'),

    /**
     * array of unsigned int 64 bit
     */
    arrayOfUint64: DataType.create('array-of-uint64', VariantTypes.aruint64, 'types/datalayer/array-of-uint64'),

    /**
     * array of float 32 bit
     */
    arrayOfFloat: DataType.create('array-of-float32', VariantTypes.arfloat, 'types/datalayer/array-of-float32'),

    /**
     * array of float 64 bit
     */
    arrayOfFloat64: DataType.create('array-of-float64', VariantTypes.ardouble, 'types/datalayer/array-of-float64'),

    /**
     * array of string (UTF-8)
     */
    arrayOfString: DataType.create('array-of-string', VariantTypes.arstring, 'types/datalayer/array-of-string'),

    /**
     * array of timestamp (Date)
     */
    arrayOfTimestamp: DataType.create('array-of-timestamp', VariantTypes.artimestamp, 'types/datalayer/array-of-timestamp'),
};

export = DATA_TYPES;