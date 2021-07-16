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
using FlatBuffers;
using sample.schema;
using System;
using System.Linq;

namespace Samples.Datalayer.Provider.Alldata
{
    /// <summary>
    /// Provides extensions methods for ProviderNodeWrapper class
    /// </summary>
    internal static class ProviderNodeWrapperExtensions
    {
        /// <summary>
        /// Changes the value of the node
        /// </summary>
        /// <param name="node">The node</param>
        public static void ChangeValue(this ProviderNodeWrapper node)
        {
            switch (node.Handler.Value.DataType)
            {
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UNKNOWN:
                    {
                        // do nothing
                        return;
                    }

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_BOOL8:
                    {
                        node.Handler.Value = new Variant(!node.Handler.Value.ToBool());
                        return;
                    }

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT8:
                    {
                        node.Handler.Value = new Variant((sbyte)(node.Handler.Value.ToSByte() + 1));
                        return;
                    }

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT8:
                    {
                        node.Handler.Value = new Variant((byte)(node.Handler.Value.ToByte() + 1));
                        return;
                    }

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT16:
                    {
                        node.Handler.Value = new Variant((short)(node.Handler.Value.ToInt16() + 1));
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT16:
                    {
                        node.Handler.Value = new Variant((ushort)(node.Handler.Value.ToUInt16() + 1));
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT32:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToInt32() + 1);
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT32:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToUInt32() + 1);
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT64:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToInt64() + 1);
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT64:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToUInt64() + 1);
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT32:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToFloat() + 0.1f);
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT64:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToDouble() + 0.1f);
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_STRING:
                    {
                        node.Handler.Value = new Variant(IncrementString(node.Handler.Value.ToString(), "_"));
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_BOOL8:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToBoolArray().Select(v => !v).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT8:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToSByteArray().Select(v => (sbyte)(v + 1)).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT8:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToByteArray().Select(v => (byte)(v + 1)).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT16:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToInt16Array().Select(v => (short)(v + 1)).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT16:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToUInt16Array().Select(v => (ushort)(v + 1)).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT32:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToInt32Array().Select(v => v + 1).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT32:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToUInt32Array().Select(v => v + 1).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT64:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToInt64Array().Select(v => v + 1).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT64:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToUInt64Array().Select(v => v + 1).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_FLOAT32:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToFloatArray().Select(v => v + 0.1f).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_FLOAT64:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToDoubleArray().Select(v => v + 0.1).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_STRING:
                    {
                        node.Handler.Value = new Variant(node.Handler.Value.ToStringArray().Select(v => IncrementString(v, "_")).ToArray());
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_RAW:
                    {
                        // Do nothing.
                        return;
                    }
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLATBUFFERS:
                    {
                        var oldInertialValue = InertialValue.GetRootAsInertialValue(node.Handler.Value.ToFlatbuffers());
                        var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
                        InertialValue.StartInertialValue(builder);
                        InertialValue.AddX(builder, (short)(oldInertialValue.X + 1));
                        InertialValue.AddY(builder, (short)(oldInertialValue.Y + 2));
                        InertialValue.AddZ(builder, (short)(oldInertialValue.Z + 3));
                        var newInertialValue = InertialValue.EndInertialValue(builder);
                        builder.Finish(newInertialValue.Value);
                        node.Handler.Value = new Variant(builder);
                        return;
                    }
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        private static string IncrementString(string text, string separator)
        {
            var separatorIndex = text.IndexOf(separator, StringComparison.Ordinal);
            var prefix = text.Substring(0, separatorIndex);
            var postfix = text.Substring(separatorIndex + 1);
            var number = uint.Parse(postfix);

            return $"{prefix}{separator}{++number}";
        }
    }
}