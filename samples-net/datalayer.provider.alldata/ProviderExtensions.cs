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

namespace Samples.Datalayer.Provider.Alldata
{
    /// <summary>
    /// Represents a provider leaf node with an 1:1 relation to an address
    /// </summary>
    internal record ProviderNodeWrapper(string Address, IProviderValueNodeHandler Handler);

    /// <summary>
    /// Provides extensions methods for IProvider
    /// </summary>
    internal static class ProviderExtensions
    {
        const string Samples = "samples";

        const string Alldata = "all-data";
        const string Static = "static";
        const string Dynamic = "dynamic";

        const string Bool8 = "bool8";
        const string Int8 = "int8";
        const string Uint8 = "uint8";
        const string Int16 = "int16";
        const string Uint16 = "uint16";
        const string Int32 = "int32";
        const string Uint32 = "uint32";
        const string Int64 = "int64";
        const string Uint64 = "uint64";
        const string Float32 = "float32";
        const string Float64 = "float64";
        const string String = "string";
        const string Flatbuffers = "fbs";

        const string ArrayOfBool8 = "array-of-bool8";
        const string ArrayOfInt8 = "array-of-int8";
        const string ArrayOfUint8 = "array-of-uint8";
        const string ArrayOfInt16 = "array-of-int16";
        const string ArrayOfUint16 = "array-of-uint16";
        const string ArrayOfInt32 = "array-of-int32";
        const string ArrayOfUint32 = "array-of-uint32";
        const string ArrayOfInt64 = "array-of-int64";
        const string ArrayOfUint64 = "array-of-uint64";
        const string ArrayOfFloat32 = "array-of-float32";
        const string ArrayOfFloat64 = "array-of-float64";
        const string ArrayOfString = "array-of-string";

        /// <summary>
        /// Creates static provider nodes with read-write support
        /// </summary>
        /// <param name="provider">The provider instance</param>
        /// <returns>Array of ProviderNodes</returns>
        internal static ProviderNodeWrapper[] CreateStaticNodes(this IProvider provider)
        {
            // Create a Flatbuffers value of type 'InertialValue'
            var builder = new FlatBufferBuilder(initialSize: Variant.DefaultFlatbuffersInitialSize);
            InertialValue.StartInertialValue(builder: builder);
            InertialValue.AddX(builder: builder, x: 30);
            InertialValue.AddY(builder: builder, y: -442);
            InertialValue.AddZ(builder: builder, z: 911);
            var inertialValue = InertialValue.EndInertialValue(builder: builder);
            builder.Finish(rootTable: inertialValue.Value);
            var variantFlatbuffers = new Variant(builder);

            // Define the root address of all static nodes with read and write support
            var root = $"{Samples}/{Alldata}/{Static}";

            // Create the static nodes with initial values
            var staticNodes = new[]
            {
                CreateNode(provider, address: $"{root}/{Bool8}", handler: new ReadWriteNodeHandler(new Variant(false))),
                CreateNode(provider, address: $"{root}/{Int8}", handler: new ReadWriteNodeHandler(new Variant(sbyte.MaxValue))),
                CreateNode(provider, address: $"{root}/{Uint8}", handler: new ReadWriteNodeHandler(new Variant(byte.MaxValue))),
                CreateNode(provider, address: $"{root}/{Int16}", handler: new ReadWriteNodeHandler(new Variant(short.MaxValue))),
                CreateNode(provider, address: $"{root}/{Uint16}", handler: new ReadWriteNodeHandler(new Variant( ushort.MaxValue))),
                CreateNode(provider, address: $"{root}/{Int32}", handler: new ReadWriteNodeHandler(new Variant( int.MaxValue))),
                CreateNode(provider, address: $"{root}/{Uint32}", handler: new ReadWriteNodeHandler( new Variant(uint.MaxValue))),
                CreateNode(provider, address: $"{root}/{Int64}", handler: new ReadWriteNodeHandler(new Variant( long.MaxValue))),
                CreateNode(provider, address: $"{root}/{Uint64}", handler: new ReadWriteNodeHandler(new Variant(ulong.MaxValue))),
                CreateNode(provider, address: $"{root}/{Float32}", handler: new ReadWriteNodeHandler(new Variant(float.MaxValue))),
                CreateNode(provider, address: $"{root}/{Float64}", handler: new ReadWriteNodeHandler(new Variant(double.MaxValue))),
                CreateNode(provider, address: $"{root}/{String}", handler: new ReadWriteNodeHandler(new Variant("This is a string"))),

                CreateNode(provider, address: $"{root}/{Flatbuffers}", handler: new ReadWriteNodeHandler(variantFlatbuffers)),

                CreateNode(provider, address: $"{root}/{ArrayOfBool8}", handler: new ReadWriteNodeHandler(new Variant(new bool[] {true, false, true}))),
                CreateNode(provider, address: $"{root}/{ArrayOfInt8}", handler: new ReadWriteNodeHandler(new Variant(new sbyte[] { sbyte.MinValue, -1, 0, sbyte.MaxValue }))),
                CreateNode(provider,  address: $"{root}/{ArrayOfUint8}", handler: new ReadWriteNodeHandler(new Variant(new byte[] {byte.MinValue, byte.MaxValue}))),
                CreateNode(provider, address: $"{root}/{ArrayOfInt16}", handler: new ReadWriteNodeHandler(new Variant(new short[] {short.MinValue, -1, 0, short.MaxValue}))),
                CreateNode(provider, address: $"{root}/{ArrayOfUint16}", handler: new ReadWriteNodeHandler(new Variant(new ushort[] {ushort.MinValue, ushort.MaxValue}))),
                CreateNode(provider, address: $"{root}/{ArrayOfInt32}", handler: new ReadWriteNodeHandler(new Variant(new int[] {int.MinValue, -1, 0, int.MaxValue}))),
                CreateNode(provider, address: $"{root}/{ArrayOfUint32}", handler: new ReadWriteNodeHandler(new Variant(new uint[] {uint.MinValue, uint.MaxValue}))),
                CreateNode(provider, address: $"{root}/{ArrayOfInt64}", handler: new ReadWriteNodeHandler(new Variant(new long[] { long.MinValue, -1, 0, long.MaxValue }))),
                CreateNode(provider, address: $"{root}/{ArrayOfUint64}", handler: new ReadWriteNodeHandler(new Variant(new ulong[] { ulong.MinValue, ulong.MaxValue }))),
                CreateNode(provider,  address: $"{root}/{ArrayOfFloat32}",  handler: new ReadWriteNodeHandler(new Variant(new float[] {float.MinValue, -1.0f, 0.0f, float.MaxValue}))),
                CreateNode(provider,  address: $"{root}/{ArrayOfFloat64}",  handler: new ReadWriteNodeHandler(new Variant(new double[] { double.MinValue, -1.0, 0.0, double.MaxValue }))),
                CreateNode(provider,  address: $"{root}/{ArrayOfString}",  handler: new ReadWriteNodeHandler(new Variant(new string[] { "Blue", "Red", "Orange", "Yellow" }))),
            };
            return staticNodes;
        }

        /// <summary>
        /// Creates dynamic provider nodes with read-only support
        /// </summary>
        /// <param name="provider">The provider instance</param>
        /// <returns>Array of ProviderNodes</returns>
        internal static ProviderNodeWrapper[] CreateDynamicNodes(this IProvider provider)
        {
            // Create a Flatbuffers value of type 'InertialValue'
            var builder = new FlatBufferBuilder(initialSize: Variant.DefaultFlatbuffersInitialSize);
            InertialValue.StartInertialValue(builder: builder);
            InertialValue.AddX(builder: builder, x: 0);
            InertialValue.AddY(builder: builder, y: 0);
            InertialValue.AddZ(builder: builder, z: 0);
            var inertialValue = InertialValue.EndInertialValue(builder: builder);
            builder.Finish(rootTable: inertialValue.Value);
            var variantFlatbuffers = new Variant(builder);

            // Define the root address of all dynamic nodes
            var root = $"{Samples}/{Alldata}/{Dynamic}";

            // Create the dynamic nodes with initial values
            var dynamicNodes = new[]
            {
                CreateNode(provider, address: $"{root}/{Bool8}", handler: new ReadOnlyNodeHandler(new Variant(false))),
                CreateNode(provider, address: $"{root}/{Int8}", handler: new ReadOnlyNodeHandler(new Variant(sbyte.MinValue))),
                CreateNode(provider, address: $"{root}/{Uint8}", handler: new ReadOnlyNodeHandler(new Variant(byte.MinValue))),
                CreateNode(provider, address: $"{root}/{Int16}", handler: new ReadOnlyNodeHandler(new Variant(short.MinValue))),
                CreateNode(provider, address: $"{root}/{Uint16}", handler: new ReadOnlyNodeHandler(new Variant( ushort.MinValue))),
                CreateNode(provider, address: $"{root}/{Int32}", handler: new ReadOnlyNodeHandler(new Variant( int.MinValue))),
                CreateNode(provider, address: $"{root}/{Uint32}", handler: new ReadOnlyNodeHandler( new Variant(uint.MinValue))),
                CreateNode(provider, address: $"{root}/{Int64}", handler: new ReadOnlyNodeHandler(new Variant( long.MinValue))),
                CreateNode(provider, address: $"{root}/{Uint64}", handler: new ReadOnlyNodeHandler(new Variant(ulong.MinValue))),
                CreateNode(provider, address: $"{root}/{Float32}", handler: new ReadOnlyNodeHandler(new Variant(float.MinValue))),
                CreateNode(provider, address: $"{root}/{Float64}", handler: new ReadOnlyNodeHandler(new Variant(double.MinValue))),
                CreateNode(provider, address: $"{root}/{String}", handler: new ReadOnlyNodeHandler(new Variant("String_0"))),

                CreateNode(provider, address: $"{root}/{Flatbuffers}", handler: new ReadOnlyNodeHandler(variantFlatbuffers)),

                CreateNode(provider, address: $"{root}/{ArrayOfBool8}", handler: new ReadOnlyNodeHandler(new Variant(new bool[] {true, false, true, false, true, false, true, false, true, false }))),
                CreateNode(provider, address: $"{root}/{ArrayOfInt8}", handler: new ReadOnlyNodeHandler(new Variant(new sbyte[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }))),
                CreateNode(provider,  address: $"{root}/{ArrayOfUint8}", handler: new ReadOnlyNodeHandler(new Variant(new byte[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }))),
                CreateNode(provider, address: $"{root}/{ArrayOfInt16}", handler: new ReadOnlyNodeHandler(new Variant(new short[] {-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }))),
                CreateNode(provider, address: $"{root}/{ArrayOfUint16}", handler: new ReadOnlyNodeHandler(new Variant(new ushort[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }))),
                CreateNode(provider, address: $"{root}/{ArrayOfInt32}", handler: new ReadOnlyNodeHandler(new Variant(new int[] {-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }))),
                CreateNode(provider, address: $"{root}/{ArrayOfUint32}", handler: new ReadOnlyNodeHandler(new Variant(new uint[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }))),
                CreateNode(provider, address: $"{root}/{ArrayOfInt64}", handler: new ReadOnlyNodeHandler(new Variant(new long[]{-5, -4, -3, -2, -1 , 0, 1, 2, 3, 4 }))),
                CreateNode(provider, address: $"{root}/{ArrayOfUint64}", handler: new ReadOnlyNodeHandler(new Variant(new ulong[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }))),
                CreateNode(provider,  address: $"{root}/{ArrayOfFloat32}",  handler: new ReadOnlyNodeHandler(new Variant(new float[] {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f }))),
                CreateNode(provider,  address: $"{root}/{ArrayOfFloat64}",  handler: new ReadOnlyNodeHandler(new Variant(new double[] {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 }))),
                CreateNode(provider,  address: $"{root}/{ArrayOfString}",  handler: new ReadOnlyNodeHandler(new Variant(new string[] { "String_0", "String_1", "String_2", "String_3", "String_4", "String_5", "String_6", "String_7", "String_8", "String_9" })))
            };

            return dynamicNodes;
        }

        /// <summary>
        /// Creates and registers a node on a given address with a handler
        /// </summary>
        /// <param name="provider">The provider instance</param>
        /// <param name="handler">The handler instance</param>
        /// <param name="address">The node address</param>
        /// <param name="value">The value of type Variant</param>
        /// <returns>The created node wrapper.</returns>
        internal static ProviderNodeWrapper CreateNode(this IProvider provider, string address, IProviderValueNodeHandler handler)
        {
            var (result, _) = provider.RegisterNode(address, handler);
            if (result.IsBad())
            {
                return null;
            }

            Console.WriteLine($"Registered node with address='{address}', result='{result}'");
            return new ProviderNodeWrapper(address, handler);
        }
    }
}