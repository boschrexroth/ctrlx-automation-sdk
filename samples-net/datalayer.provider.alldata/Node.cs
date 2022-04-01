/*
MIT License

Copyright (c) 2021 Bosch Rexroth AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

using Datalayer;

namespace Samples.Datalayer.Provider.Alldata
{
    using FlatBuffers;
    using sample.schema;
    using System;
    using System.Linq;

    /// <summary>
    /// Represents a node with an address and a value.
    /// </summary>
    internal class Node
    {
        /// <summary>
        /// Gets the Address.
        /// </summary>
        public string Address { get; }

        /// <summary>
        /// Gets or sets the Value.
        /// </summary>
        public IVariant Value { get; set; }

        /// <summary>
        /// Gets the Metadata.
        /// </summary>
        public IVariant Metadata { get; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Node"/> class.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <param name="metadata">The metadata<see cref="IVariant"/>.</param>
        public Node(string address, IVariant value, IVariant metadata)
        {
            Address = address;
            Value = value;
            Metadata = metadata;
        }

        /// <summary>
        /// The ChangeValue.
        /// </summary>
        public void ChangeValue()
        {
            switch (Value.DataType)
            {
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UNKNOWN:
                    {
                        // do nothing
                        break;
                    }

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_BOOL8:
                    {
                        Value = new Variant(!Value.ToBool());
                        break;
                    }

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT8:
                    {
                        Value = new Variant((sbyte)(Value.ToSByte() + 1));
                        break;
                    }

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT8:
                    {
                        Value = new Variant((byte)(Value.ToByte() + 1));
                        break;
                    }

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT16:
                    {
                        Value = new Variant((short)(Value.ToInt16() + 1));
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT16:
                    {
                        Value = new Variant((ushort)(Value.ToUInt16() + 1));
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT32:
                    {
                        Value = new Variant(Value.ToInt32() + 1);
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT32:
                    {
                        Value = new Variant(Value.ToUInt32() + 1);
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT64:
                    {
                        Value = new Variant(Value.ToInt64() + 1L);
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT64:
                    {
                        Value = new Variant(Value.ToUInt64() + 1L);
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT32:
                    {
                        Value = new Variant(Value.ToFloat() + .1f);
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT64:
                    {
                        Value = new Variant(Value.ToDouble() + 0.1);
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_STRING:
                    {
                        Value = new Variant(IncrementString(Value.ToString(), "_"));
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_BOOL8:
                    {
                        Value = new Variant(Value.ToBoolArray().Select(v => !v).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT8:
                    {
                        Value = new Variant(Value.ToSByteArray().Select(v => (sbyte)(v + 1)).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT8:
                    {
                        Value = new Variant(Value.ToByteArray().Select(v => (byte)(v + 1)).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT16:
                    {
                        Value = new Variant(Value.ToInt16Array().Select(v => (short)(v + 1)).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT16:
                    {
                        Value = new Variant(Value.ToUInt16Array().Select(v => (ushort)(v + 1)).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT32:
                    {
                        Value = new Variant(Value.ToInt32Array().Select(v => v + 1).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT32:
                    {
                        Value = new Variant(Value.ToUInt32Array().Select(v => v + 1).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT64:
                    {
                        Value = new Variant(Value.ToInt64Array().Select(v => v + 1L).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT64:
                    {
                        Value = new Variant(Value.ToUInt64Array().Select(v => v + 1L).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_FLOAT32:
                    {
                        Value = new Variant(Value.ToFloatArray().Select(v => v + .1f).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_FLOAT64:
                    {
                        Value = new Variant(Value.ToDoubleArray().Select(v => v + .1).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_STRING:
                    {
                        Value = new Variant(Value.ToStringArray().Select(v => IncrementString(v, "_")).ToArray());
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_RAW:
                    {
                        // Do nothing.
                        break;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLATBUFFERS:
                    {
                        var oldInertialValue = InertialValue.GetRootAsInertialValue(Value.ToFlatbuffers());
                        Value = CreateInertialValue(
                            (short)(oldInertialValue.X + 1),
                            (short)(oldInertialValue.Y + 2),
                            (short)(oldInertialValue.Z + 3));
                        break;
                    }
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        /// <summary>
        /// The IncrementString.
        /// </summary>
        /// <param name="text">The text<see cref="string"/>.</param>
        /// <param name="separator">The separator<see cref="string"/>.</param>
        /// <returns>The <see cref="string"/>.</returns>
        private static string IncrementString(string text, string separator)
        {
            var separatorIndex = text.IndexOf(separator, StringComparison.Ordinal);
            var prefix = text.Substring(0, separatorIndex);
            var postfix = text.Substring(separatorIndex + 1);
            var number = uint.Parse(postfix);

            return $"{prefix}{separator}{++number}";
        }

        private static Variant CreateInertialValue(short x, short y, short z)
        {
            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
            var offset = InertialValue.CreateInertialValue(builder, x, y, z);
            builder.Finish(offset.Value);
            return new Variant(builder);
        }
    }
}
