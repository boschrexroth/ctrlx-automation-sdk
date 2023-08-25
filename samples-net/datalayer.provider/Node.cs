/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

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
    }
}
