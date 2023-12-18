/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;

namespace Samples.Datalayer.Provider.Alldata
{
    using comm.datalayer;
    using Google.FlatBuffers;
    using sample.schema;
    using System;
    using System.Linq;

    /// <summary>
    /// Represents a node with an address and a value.
    /// </summary>
    internal class Node
    {

        // Please define node folder names in ctrlX Data Layer
        private const string Root = "sdk/net/provider/all-data";
        private const string Static = "static";
        private const string Dynamic = "dynamic";
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
        /// Changes the value.
        /// <param name="utcNow">The current time (UTC)<see cref="DateTime"/>.</param>
        /// </summary>
        public void ChangeValue(DateTime utcNow)
        {
            switch (Value.DataType)
            {
                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UNKNOWN:
                    // do nothing
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_BOOL8:
                    Value = new Variant(!Value.ToBool());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT8:
                    Value = new Variant((sbyte)(Value.ToSByte() + 1));
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT8:
                    Value = new Variant((byte)(Value.ToByte() + 1));
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT16:
                    Value = new Variant((short)(Value.ToInt16() + 1));
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT16:
                    Value = new Variant((ushort)(Value.ToUInt16() + 1));
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT32:
                    Value = new Variant(Value.ToInt32() + 1);
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT32:
                    Value = new Variant(Value.ToUInt32() + 1);
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_INT64:
                    Value = new Variant(Value.ToInt64() + 1L);
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_UINT64:
                    Value = new Variant(Value.ToUInt64() + 1L);
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT32:
                    Value = new Variant(Value.ToFloat() + .1f);
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLOAT64:
                    Value = new Variant(Value.ToDouble() + 0.1);
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_STRING:
                    Value = new Variant(IncrementString(Value.ToString(), "_"));
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_TIMESTAMP:
                    Value = new Variant(utcNow);
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_BOOL8:
                    Value = new Variant(Value.ToBoolArray().Select(v => !v).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT8:
                    Value = new Variant(Value.ToSByteArray().Select(v => (sbyte)(v + 1)).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT8:
                    Value = new Variant(Value.ToByteArray().Select(v => (byte)(v + 1)).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT16:
                    Value = new Variant(Value.ToInt16Array().Select(v => (short)(v + 1)).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT16:
                    Value = new Variant(Value.ToUInt16Array().Select(v => (ushort)(v + 1)).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT32:
                    Value = new Variant(Value.ToInt32Array().Select(v => v + 1).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT32:
                    Value = new Variant(Value.ToUInt32Array().Select(v => v + 1).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_INT64:
                    Value = new Variant(Value.ToInt64Array().Select(v => v + 1L).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_UINT64:
                    Value = new Variant(Value.ToUInt64Array().Select(v => v + 1L).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_FLOAT32:
                    Value = new Variant(Value.ToFloatArray().Select(v => v + .1f).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_FLOAT64:
                    Value = new Variant(Value.ToDoubleArray().Select(v => v + .1).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_STRING:
                    Value = new Variant(Value.ToStringArray().Select(v => IncrementString(v, "_")).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_ARRAY_OF_TIMESTAMP:
                    Value = new Variant(Value.ToDateTimeArray().Select(_ => utcNow).ToArray());
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_FLATBUFFERS:
                    var oldInertialValue = InertialValue.GetRootAsInertialValue(Value.ToFlatbuffers());
                    Value = CreateInertialValue(
                        (short)(oldInertialValue.X + 1),
                        (short)(oldInertialValue.Y + 2),
                        (short)(oldInertialValue.Z + 3));
                    break;

                case DLR_VARIANT_TYPE.DLR_VARIANT_TYPE_RAW:
                    // Do nothing.
                    break;

                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        /// <summary>
        /// Increments a string
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

        /// <summary>
        /// Creates an 'InertialValue' flatbuffers.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y">The separator<see cref="string"/>.</param>
        /// <param name="z">The separator<see cref="string"/>.</param>
        /// <returns>The Variant containing the flatbuffers.</returns>
        public static IVariant CreateInertialValue(short x, short y, short z)
        {
            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
            var offset = InertialValue.CreateInertialValue(builder, x, y, z);
            builder.Finish(offset.Value);
            return new Variant(builder);
        }


        /// <summary>
        /// Creates a static node.
        /// </summary>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <param name="dataType">The dataType<see cref="DataType"/>.</param>
        /// <returns>The <see cref="IProviderNode"/>.</returns>
        public static Node CreateStatic(IVariant value, DataType dataType)
        {
            var address = $"{Root}/{Static}/{dataType.Name}";
            var description = $"{Static} data with type {dataType.Name}";

            var metaData = new MetadataBuilder(AllowedOperationFlags.Read | AllowedOperationFlags.Write, description)
                .SetNodeClass(NodeClass.Variable)
                .AddReference(ReferenceType.ReadType, dataType.Address)
                .AddReference(ReferenceType.WriteType, dataType.Address)
                .Build();

            return new Node(address, value, metaData);
        }

        /// <summary>
        /// Creates a dynamic node.
        /// </summary>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <param name="dataType">The dataType<see cref="DataType"/>.</param>
        public static Node CreateDynamic(IVariant value, DataType dataType)
        {
            var address = $"{Root}/{Dynamic}/{dataType.Name}";
            var description = $"{Dynamic} data with type {dataType.Name}";

            var metaData = new MetadataBuilder(AllowedOperationFlags.Read, description)
                  .SetNodeClass(NodeClass.Variable)
                  .AddReference(ReferenceType.ReadType, dataType.Address)
                  .Build();

            return new Node(address, value, metaData);
        }
    }
}
