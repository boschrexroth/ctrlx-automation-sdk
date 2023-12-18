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
        /// The CreateNode.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="targetAddress">The targetAddress<see cref="string"/>.</param>
        /// <param name="description">The description<see cref="string"/>.</param>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <returns>The <see cref="Node"/>.</returns>
        public static Node CreateReadOnlyNode(string address, string targetAddress, string description, IVariant value)
        {
            var metaData = new MetadataBuilder(AllowedOperationFlags.Read, description)
                .SetNodeClass(NodeClass.Variable)
                .AddReference(ReferenceType.ReadType, targetAddress)
                .Build();

            return new Node(address, value, metaData);
        }

        /// <summary>
        /// The CreateReadWriteNode.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="targetAddress">The targetAddress<see cref="string"/>.</param>
        /// <param name="description">The description<see cref="string"/>.</param>
        /// <param name="value">The value<see cref="IVariant"/>.</param>
        /// <returns>The <see cref="Node"/>.</returns>
        public static Node CreateReadWriteNode(string address, string targetAddress, string description, IVariant value)
        {
            var metaData = new MetadataBuilder(AllowedOperationFlags.Read | AllowedOperationFlags.Write, description)
                .SetNodeClass(NodeClass.Variable)
                .AddReference(ReferenceType.ReadType, targetAddress)
                .AddReference(ReferenceType.WriteType, targetAddress)
                .Build();

            return new Node(address, value, metaData);
        }
    }
}
