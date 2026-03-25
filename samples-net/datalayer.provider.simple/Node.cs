/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using comm.datalayer;
using Datalayer;

namespace Samples.Datalayer.Provider
{
    /// <summary>
    /// Represents a node with an address and a value.
    /// </summary>
    /// <remarks>
    /// Initializes a new instance of the <see cref="Node"/> class.
    /// </remarks>
    /// <param name="address">The address<see cref="string"/>.</param>
    /// <param name="typeAddress">The address if the type<see cref="string"/>.</param>
    /// <param name="value">The value<see cref="IVariant"/>.</param>
    /// <param name="metadata">The metadata<see cref="IVariant"/>.</param>
    internal class Node(string address, string typeAddress, IVariant value, IVariant metadata)
    {
        /// <summary>
        /// Gets the Address.
        /// </summary>
        public string Address { get; } = address;

        /// <summary>
        /// Gets the Address of the Type.
        /// </summary>
        public string TypeAddress { get; } = typeAddress;

        /// <summary>
        /// Gets or sets the Value.
        /// </summary>
        public IVariant Value { get; set; } = value;

        /// <summary>
        /// Gets the Metadata.
        /// </summary>
        public IVariant Metadata { get; } = metadata;

        /// <summary>
        /// Creates a read-only node.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="typeAddress">The address if the type<see cref="string"/>.</param>
        /// <param name="description">The description<see cref="string"/>.</param>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <returns>The <see cref="Node"/>.</returns>
        public static Node CreateReadOnlyNode(string address, string typeAddress, string description, IVariant value)
        {
            // Please see here for recommended allowed operations by node type: 
            // https://docs.automation.boschrexroth.com/doc/1925281162/metadata/latest/en/

            // Build Metadata using generated MetadataT type.
            var operations = new AllowedOperationsT
            {
                Read = true,
                Browse = false // Default: true
            };

            var readReference = new ReferenceT
            {
                Type = ReferenceType.ReadType.ToString(),
                TargetAddress = typeAddress
            };

            var md = new MetadataT
            {
                Operations = operations,        // Mandatory
                Description = description,      // Mandatory (but can be left empty)
                DescriptionUrl = "",            // Mandatory (but can be left empty)
                NodeClass = NodeClass.Variable,
                References = [readReference],
            };
            var metaData = new Variant(md);

            // Alternatively we can use the MetadataBuilder.

            // var metaData = new MetadataBuilder(AllowedOperationFlags.Read, description)
            //     .SetNodeClass(NodeClass.Variable)
            //     .AddReference(ReferenceType.ReadType, typeAddress)
            //     .Build();

            return new Node(address, typeAddress, value, metaData);
        }

        /// <summary>
        /// Creates a readable and writable node.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="typeAddress">The address if the type<see cref="string"/>.</param>
        /// <param name="description">The description<see cref="string"/>.</param>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <returns>The <see cref="Node"/>.</returns>
        public static Node CreateReadWriteNode(string address, string typeAddress, string description, IVariant value)
        {
            // Please see here for recommended allowed operations by node type: 
            // https://docs.automation.boschrexroth.com/doc/1925281162/metadata/latest/en/ 

            // Build Metadata using generated MetadataT type.
            var operations = new AllowedOperationsT
            {
                Read = true,
                Write = true,
                Browse = false // Default: true
            };

            var readReference = new ReferenceT
            {
                Type = ReferenceType.ReadType.ToString(),
                TargetAddress = typeAddress
            };

            var writeReference = new ReferenceT
            {
                Type = ReferenceType.WriteType.ToString(),
                TargetAddress = typeAddress
            };

            var md = new MetadataT
            {
                Operations = operations,        // Mandatory
                Description = description,      // Mandatory (but can be left empty)
                DescriptionUrl = "",            // Mandatory (but can be left empty)
                NodeClass = NodeClass.Variable,
                References = [readReference, writeReference],
            };
            var metaData = new Variant(md);

            // Alternatively we can use the MetadataBuilder.

            // var metaData = new MetadataBuilder(AllowedOperationFlags.Read | AllowedOperationFlags.Write, description)
            //     .SetNodeClass(NodeClass.Variable)
            //     .AddReference(ReferenceType.ReadType, typeAddress)
            //     .AddReference(ReferenceType.WriteType, typeAddress)
            //     .Build();

            return new Node(address, typeAddress, value, metaData);
        }
    }
}
