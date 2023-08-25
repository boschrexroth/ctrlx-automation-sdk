/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;
using System;
using System.Text.Json;

namespace Samples.Datalayer.MQTT.Base
{
    /// <summary>
    /// Provides extensions methods for IVariant
    /// </summary>
    internal static class VariantExtensions
    {
        /// <summary>
        /// Trims the Variant if it's a string, else returns Variant AS-IT-IS
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static IVariant Trim(this IVariant value)
        {
            if (!value.IsString)
            {
                return value;
            }

            return new Variant(value.ToString().Trim());
        }

        /// <summary>
        /// Rounds the Variant to the nearest integral number (Int32), if it's a floating point number, else returns the Variant AS-IT-IS
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static IVariant Round(this IVariant value)
        {
            return value?.DataType switch
            {
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT32 => new Variant((int)Math.Round(value.ToFloat())),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT64 => new Variant((int)Math.Round(value.ToDouble())),
                _ => value,
            };
        }

        /// <summary>
        ///  Returns a Variant with the largest integral value (Int32) less than or equal to the specified floating-point number, else returns the Variant AS-IT-IS.
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static IVariant Floor(this IVariant value)
        {
            return value?.DataType switch
            {
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT32 => new Variant((int)Math.Floor(value.ToFloat())),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT64 => new Variant((int)Math.Floor(value.ToDouble())),
                _ => value,
            };
        }

        /// <summary>
        ///  Returns a Variant with the smallest integral value (Int32) that is greater than or equal to the specified floating-point number, else returns the Variant AS-IT-IS.
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static IVariant Ceiling(this IVariant value)
        {
            return value?.DataType switch
            {
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT32 => new Variant(Math.Ceiling(value.ToFloat())),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT64 => new Variant(Math.Ceiling(value.ToDouble())),
                _ => value,
            };
        }

        /// <summary>
        /// Indicates whether the Variant is a string containing null, empty, or consists only of white-space characters.
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static bool IsEmptyOrWhiteSpace(this IVariant value)
        {
            if (!value.IsString)
            {
                return false;
            }

            return string.IsNullOrWhiteSpace(value?.ToString());
        }

        /// <summary>
        /// Returns the JSON representation of the Variant value
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static string ToJson(this IVariant value)
        {
            //We have to use JSON serializer, because toString() is not properly implemented for numeric data types
            return value?.DataType switch
            {
                //Scalars -> Stringify
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_BOOL8 => JsonSerializer.Serialize(value.ToBool()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT8 => JsonSerializer.Serialize(value.ToSByte()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT8 => JsonSerializer.Serialize(value.ToByte()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT16 => JsonSerializer.Serialize(value.ToInt16()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT16 => JsonSerializer.Serialize(value.ToUInt16()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT32 => JsonSerializer.Serialize(value.ToInt32()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT32 => JsonSerializer.Serialize(value.ToUInt32()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT64 => JsonSerializer.Serialize(value.ToInt64()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT64 => JsonSerializer.Serialize(value.ToUInt64()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT32 => JsonSerializer.Serialize(value.ToFloat()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT64 => JsonSerializer.Serialize(value.ToDouble()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_STRING => JsonSerializer.Serialize(value.ToString()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_TIMESTAMP => JsonSerializer.Serialize(value.ToDateTime()),

                //Flatbuffer -> JSON Array
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLATBUFFERS => JsonSerializer.Serialize(value.ToFlatbuffers().ToSizedArray()),

                //Array -> JSON Array
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_RAW => JsonSerializer.Serialize(value.ToRawByteArray()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_BOOL8 => JsonSerializer.Serialize(value.ToBoolArray()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT8 => JsonSerializer.Serialize(value.ToSByteArray()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT8 => JsonSerializer.Serialize(value.ToByteArray()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT16 => JsonSerializer.Serialize(value.ToInt16Array()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT16 => JsonSerializer.Serialize(value.ToUInt16Array()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT32 => JsonSerializer.Serialize(value.ToInt32Array()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT32 => JsonSerializer.Serialize(value.ToUInt32Array()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT64 => JsonSerializer.Serialize(value.ToInt64Array()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT64 => JsonSerializer.Serialize(value.ToUInt64Array()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_FLOAT32 => JsonSerializer.Serialize(value.ToFloatArray()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_FLOAT64 => JsonSerializer.Serialize(value.ToDoubleArray()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_STRING => JsonSerializer.Serialize(value.ToStringArray()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_TIMESTAMP => JsonSerializer.Serialize(value.ToDateTimeArray()),
                DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UNKNOWN => null,
                _ => null,
            };
        }
    }
}