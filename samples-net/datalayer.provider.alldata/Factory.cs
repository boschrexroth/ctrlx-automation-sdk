/*
MIT License

Copyright (c) 2021 Bosch Rexroth AG

Permission is hereby granted, free charge, to any person obtaining a copy
this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions the Software.

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
    using comm.datalayer;
    using FlatBuffers;
    using sample.schema;

    /// <summary>
    /// Provides extensions methods for IProvider.
    /// </summary>
    internal static class Factory
    {
        /// <summary>
        /// Defines the Samples.
        /// </summary>
        internal const string Samples = "samples";

        /// <summary>
        /// Defines the AllData.
        /// </summary>
        internal const string AllData = "all-data";

        /// <summary>
        /// Defines the Static.
        /// </summary>
        internal const string Static = "static";

        /// <summary>
        /// Defines the Dynamic.
        /// </summary>
        internal const string Dynamic = "dynamic";

        /// <summary>
        /// The CreateStaticNode.
        /// </summary>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <param name="dataType">The dataType<see cref="DataType"/>.</param>
        /// <returns>The <see cref="IProviderNode"/>.</returns>
        public static Node CreateStaticNode(IVariant value, DataType dataType)
        {
            var address = $"{Samples}/{AllData}/{Static}/{dataType.Name}";
            var description = $"{Static} data with type {dataType.Name}";

            var metaData = new MetadataBuilder()
                .SetNodeClass(NodeClass.Variable)
                .SetDescription(description)
                .SetAllowedOperations(true, true, false, false, false)
                .AddReference("readType", dataType.Address)
                .AddReference("writeType", dataType.Address)
                .Build();

            return new Node(address, value, metaData);
        }

        /// <summary>
        /// The CreateDynamicNode.
        /// </summary>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <param name="dataType">The dataType<see cref="DataType"/>.</param>
        public static Node CreateDynamicNode(IVariant value, DataType dataType)
        {
            var address = $"{Samples}/{AllData}/{Dynamic}/{dataType.Name}";
            var description = $"{Dynamic} data with type {dataType.Name}";

            var metaData = new MetadataBuilder()
                .SetNodeClass(NodeClass.Variable)
                .SetDescription(description)
                .SetAllowedOperations(true, false, false, false, false)
                .AddReference("readType", dataType.Address)
                .Build();


            return new Node(address, value, metaData);
        }

        /// <summary>
        /// The CreateInertialValue.
        /// </summary>
        /// <param name="x">The x<see cref="short"/>.</param>
        /// <param name="y">The y<see cref="short"/>.</param>
        /// <param name="z">The z<see cref="short"/>.</param>
        /// <returns>The <see cref="Variant"/>.</returns>
        public static Variant CreateInertialValue(short x, short y, short z)
        {
            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
            var offset = InertialValue.CreateInertialValue(builder, x, y, z);
            builder.Finish(offset.Value);
            return new Variant(builder);
        }
    }
}
