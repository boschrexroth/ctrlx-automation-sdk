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

namespace Samples.Datalayer.Provider
{
    using comm.datalayer;
    using FlatBuffers;
    using System.Collections.Generic;

    /// <summary>
    /// Defines the <see cref="MetadataBuilder" />.
    /// </summary>
    internal class MetadataBuilder
    {
        /// <summary>
        /// Defines the _builder.
        /// </summary>
        private readonly FlatBufferBuilder _builder;

        /// <summary>
        /// Defines the _references.
        /// </summary>
        private readonly List<Offset<Reference>> _references;

        /// <summary>
        /// Defines the _extensions.
        /// </summary>
        private readonly List<Offset<Extension>> _extensions;

        /// <summary>
        /// Defines the _operations.
        /// </summary>
        private Offset<AllowedOperations> _operations;

        /// <summary>
        /// Defines the _description.
        /// </summary>
        private StringOffset _description;

        /// <summary>
        /// Defines the _descriptionUrl.
        /// </summary>
        private StringOffset _descriptionUrl;

        /// <summary>
        /// Defines the _nodeClass.
        /// </summary>
        private NodeClass _nodeClass;

        /// <summary>
        /// Defines the _displayName.
        /// </summary>
        private StringOffset _displayName;

        /// <summary>
        /// Defines the _displayFormat.
        /// </summary>
        private DisplayFormat _displayFormat;

        /// <summary>
        /// Defines the _unit.
        /// </summary>
        private StringOffset _unit;

        /// <summary>
        /// Initializes a new instance of the <see cref="MetadataBuilder"/> class.
        /// </summary>
        public MetadataBuilder()
        {
            _builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);
            _references = new List<Offset<Reference>>();
            _extensions = new List<Offset<Extension>>();
            _operations = AllowedOperations.CreateAllowedOperations(_builder);
            _description = _builder.CreateString(string.Empty);
            _descriptionUrl = _builder.CreateString(string.Empty);
            _displayName = _builder.CreateString(string.Empty);
            _displayFormat = DisplayFormat.Auto;
            _unit = _builder.CreateString(string.Empty);
        }

        /// <summary>
        /// The SetNodeClass.
        /// </summary>
        /// <param name="nodeClass">The nodeClass<see cref="NodeClass"/>.</param>
        /// <returns>The <see cref="MetadataBuilder"/>.</returns>
        public MetadataBuilder SetNodeClass(NodeClass nodeClass)
        {
            _nodeClass = nodeClass;
            return this;
        }

        /// <summary>
        /// The SetDescription.
        /// </summary>
        /// <param name="description">The description<see cref="string"/>.</param>
        /// <returns>The <see cref="MetadataBuilder"/>.</returns>
        public MetadataBuilder SetDescription(string description)
        {
            _description = _builder.CreateString(description);
            return this;
        }

        /// <summary>
        /// The SetDescriptionUrl.
        /// </summary>
        /// <param name="descriptionUrl">The descriptionUrl<see cref="string"/>.</param>
        /// <returns>The <see cref="MetadataBuilder"/>.</returns>
        public MetadataBuilder SetDescriptionUrl(string descriptionUrl)
        {
            _descriptionUrl = _builder.CreateString(descriptionUrl);
            return this;
        }

        /// <summary>
        /// The SetDisplayName.
        /// </summary>
        /// <param name="displayName">The displayName<see cref="string"/>.</param>
        /// <returns>The <see cref="MetadataBuilder"/>.</returns>
        public MetadataBuilder SetDisplayName(string displayName)
        {
            _displayName = _builder.CreateString(displayName);
            return this;
        }

        /// <summary>
        /// The SetDisplayFormat.
        /// </summary>
        /// <param name="displayFormat">The displayFormat<see cref="DisplayFormat"/>.</param>
        /// <returns>The <see cref="MetadataBuilder"/>.</returns>
        public MetadataBuilder SetDisplayFormat(DisplayFormat displayFormat)
        {
            _displayFormat = displayFormat;
            return this;
        }

        /// <summary>
        /// The SetUnit.
        /// </summary>
        /// <param name="unit">The unit<see cref="string"/>.</param>
        /// <returns>The <see cref="MetadataBuilder"/>.</returns>
        public MetadataBuilder SetUnit(string unit)
        {
            _unit = _builder.CreateString(unit);
            return this;
        }

        /// <summary>
        /// The AddReference.
        /// </summary>
        /// <param name="type">The type<see cref="string"/>.</param>
        /// <param name="targetAddress">The targetAddress<see cref="string"/>.</param>
        /// <returns>The <see cref="MetadataBuilder"/>.</returns>
        public MetadataBuilder AddReference(string type, string targetAddress)
        {
            var typeOffset = _builder.CreateString(type);
            var targetAddressOffset = _builder.CreateString(targetAddress);
            var referenceOffset = Reference.CreateReference(_builder, typeOffset, targetAddressOffset);
            _references.Add(referenceOffset);
            return this;
        }

        /// <summary>
        /// The AddExtension.
        /// </summary>
        /// <param name="key">The key<see cref="string"/>.</param>
        /// <param name="value">The value<see cref="string"/>.</param>
        /// <returns>The <see cref="MetadataBuilder"/>.</returns>
        public MetadataBuilder AddExtension(string key, string value)
        {
            var keyOffset = _builder.CreateString(key);
            var valueOffset = _builder.CreateString(value);
            var extensionOffset = Extension.CreateExtension(_builder, keyOffset, valueOffset);
            _extensions.Add(extensionOffset);
            return this;
        }

        /// <summary>
        /// The SetAllowedOperations.
        /// </summary>
        /// <param name="read">The read<see cref="bool"/>.</param>
        /// <param name="write">The write<see cref="bool"/>.</param>
        /// <param name="create">The create<see cref="bool"/>.</param>
        /// <param name="delete">The delete<see cref="bool"/>.</param>
        /// <param name="browse">The browse<see cref="bool"/>.</param>
        /// <returns>The <see cref="MetadataBuilder"/>.</returns>
        public MetadataBuilder SetAllowedOperations(bool read, bool write, bool create, bool delete, bool browse)
        {
            _operations = AllowedOperations.CreateAllowedOperations(_builder, read, write, create, delete, browse);
            return this;
        }

        /// <summary>
        /// The Build.
        /// </summary>
        /// <returns>The <see cref="Variant"/>.</returns>
        public Variant Build()
        {
            var referencesVector = Metadata.CreateReferencesVector(_builder, _references.ToArray());
            var extensionsVector = Metadata.CreateExtensionsVector(_builder, _extensions.ToArray());
            var metadataOffset = Metadata.CreateMetadata(_builder,
                                                        _nodeClass,
                                                        _operations,
                                                        _description,
                                                        _descriptionUrl,
                                                        _displayName,
                                                        _displayFormat,
                                                        _unit,
                                                        extensionsVector,
                                                        referencesVector);

            _builder.Finish(metadataOffset.Value);

            return new Variant(_builder);
        }
    }
}
