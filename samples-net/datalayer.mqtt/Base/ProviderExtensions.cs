/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using comm.datalayer;
using Datalayer;
using System;

namespace Samples.Datalayer.MQTT.Base
{
    /// <summary>
    /// Represents a provider node with an 1:1 relation to an address.
    /// </summary>
    internal class ProviderNodeWrapper
    {
        /// <summary>
        /// Creates a node wrapper
        /// </summary>
        /// <param name="address"></param>
        /// <param name="name"></param>
        /// <param name="node"></param>
        private ProviderNodeWrapper(string address, string name, IProviderNode node)
        {
            Address = address;
            Name = name;
            Node = node;
        }

        /// <summary>
        /// Creates a node wrapper holding a variable
        /// </summary>
        /// <param name="address"></param>
        /// <param name="name"></param>
        /// <param name="node"></param>
        /// <param name="value"></param>
        /// <param name="readable"></param>
        /// <param name="writeable"></param>
        public ProviderNodeWrapper(string address, string name, IProviderNode node, IVariant value, bool readable, bool writeable)
            : this(address, name, node)
        {
            Value = value;
            IsReadable = readable;
            IsWriteable = writeable;
            Category = NodeClass.Variable;
        }

        /// <summary>
        /// Creates a node wrapper holding a folder or a resource (deletable)
        /// </summary>
        /// <param name="address"></param>
        /// <param name="name"></param>
        /// <param name="node"></param>
        /// <param name="deleteable"></param>
        public ProviderNodeWrapper(string address, string name, IProviderNode node, bool deleteable)
             : this(address, name, node)
        {
            Address = address;
            Name = name;
            Node = node;

            IsDeleteable = deleteable;
            IsCreatable = false;
            IsBrowsable = true;
            Category = deleteable ? NodeClass.Resource : NodeClass.Folder;
        }

        /// <summary>
        /// Gets the address
        /// </summary>
        public string Address { get; }

        /// <summary>
        /// Gets the name
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// Gets the wrapped node 
        /// </summary>
        public IProviderNode Node { get; }

        /// <summary>
        /// Gets or sets the value
        /// </summary>
        public IVariant Value { get; set; }

        /// <summary>
        /// Gets the category (node class)
        /// </summary>
        public NodeClass Category { get; }

        /// <summary>
        /// Returns whether the node can be browsed
        /// </summary>
        public bool IsBrowsable { get; }

        /// <summary>
        /// Returns whether the node can be read
        /// </summary>
        public bool IsReadable { get; }

        /// <summary>
        /// Returns whether the node can be written
        /// </summary>
        public bool IsWriteable { get; }

        /// <summary>
        /// Returns whether the node can be created
        /// </summary>
        public bool IsCreatable { get; }

        /// <summary>
        /// Returns whether the node can be deleted
        /// </summary>
        public bool IsDeleteable { get; }

        /// <summary>
        /// Returns whether the node is read only
        /// </summary>
        public bool IsReadOnly => IsReadable && !IsWriteable;

        /// <summary>
        /// Returns whether the node is write only
        /// </summary>
        public bool IsWriteOnly => !IsReadable && IsWriteable;
    }

    /// <summary>
    /// Provides extensions methods for IProvider
    /// </summary>
    internal static class ProviderExtensions
    {
        /// <summary>
        /// Creates and registeres a wrapped folder node
        /// </summary>
        /// <param name="provider"></param>
        /// <param name="handler"></param>
        /// <param name="baseAddress"></param>
        /// <param name="name"></param>
        /// <param name="deleteable"></param>
        /// <returns></returns>
        public static (DLR_RESULT, ProviderNodeWrapper) CreateBranchNode(
            this IProvider provider, IProviderNodeHandler handler, string baseAddress, string name, bool deleteable = false)
        {
            var address = $"{baseAddress}/{name}";
            var (result, node) = provider.RegisterNode(address, handler);
            if (result.IsBad())
            {
                return (result, null);
            }

            Console.WriteLine($"Created a node on registered address: {address}");
            return (result, new ProviderNodeWrapper(address, name, node, deleteable));
        }

        /// <summary>
        /// Creates and registeres a wrapped variable node
        /// </summary>
        /// <param name="provider"></param>
        /// <param name="handler"></param>
        /// <param name="baseAddress"></param>
        /// <param name="name"></param>
        /// <param name="value"></param>
        /// <param name="readable"></param>
        /// <param name="writeable"></param>
        /// <returns></returns>
        public static (DLR_RESULT, ProviderNodeWrapper) CreateVariableNode(
            this IProvider provider, IProviderNodeHandler handler, string baseAddress, string name, IVariant value, bool readable = true, bool writeable = false)
        {
            var address = $"{baseAddress}/{name}";
            var (result, node) = provider.RegisterNode(address, handler);
            if (result.IsBad())
            {
                return (result, null);
            }

            Console.WriteLine($"Created a node on registered address: {address}");
            return (result, new ProviderNodeWrapper(address, name, node, value, readable, writeable));
        }
    }
}