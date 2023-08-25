/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;
using System;

namespace Samples.Datalayer.Provider.Virtual
{

    /// <summary>
    /// Represents a provider leaf node with an 1:1 relation to an address
    /// </summary>
    internal record ProviderNodeWrapper(string Address, string Name, IProviderNode Node);

    /// <summary>
    /// Provides extensions methods for IProvider
    /// </summary>
    internal static class ProviderExtensions
    {
        /// <summary>
        /// Creates and registers a node on a given address with a handler
        /// </summary>
        /// <param name="provider">The provider instance</param>
        /// <param name="baseAddress">The base node address</param>
        /// <param name="name">The name of the node appended to the base address</param>
        /// <param name="handler">The handler instance</param>
        /// <returns>The created node wrapper.</returns>
        public static (DLR_RESULT, ProviderNodeWrapper) CreateNode(this IProvider provider, string baseAddress, string name, IProviderNodeHandler handler)
        {
            //Register the node
            var address = $"{baseAddress}/{name}";
            var (result, node) = provider.RegisterNode(address, handler);
            if (result.IsBad())
            {
                return (result, null);
            }

            Console.WriteLine($"Created a node on registered address: {address}");
            return (result, new ProviderNodeWrapper(address, name, node));
        }
    }
}