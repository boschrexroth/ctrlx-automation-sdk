/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Datalayer;

namespace Samples.Datalayer.Client.Browse
{
    using System;

    /// <summary>
    /// Provides extension methods for IClient.
    /// </summary>
    internal static class ClientExtensions
    {
        /// <summary>
        /// Defines the MaxChars.
        /// </summary>
        private const int MaxChars = 100;

        /// <summary>
        /// Browses all nodes in DataLayer tree.
        /// </summary>
        /// <param name="client">The client.</param>
        internal static void BrowseTree(this IClient client)
        {
            client.Traverse();
        }

        /// <summary>
        /// Traverse all nodes in given address and write leaf and value with indentation to console.
        /// </summary>
        /// <param name="client">The client.</param>
        /// <param name="address">The address.</param>
        /// <param name="leaf">The leaf.</param>
        /// <param name="indent">The indentation.</param>
        /// <param name="last">The last.</param>
        private static void Traverse(this IClient client,
            string address = "",
            string leaf = "",
            string indent = "",
            bool last = true)
        {
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.Write(indent);
            if (last)
            {
                Console.Write("\\-");
                indent += "  ";
            }
            else
            {
                Console.Write("|-");
                indent += "| ";
            }

            Console.ForegroundColor = ConsoleColor.Green;
            Console.Write($"[{leaf}] ");

            var valueAsString = client.GetValue(address);

            Console.ForegroundColor = valueAsString.StartsWith("error:") ? ConsoleColor.Red : ConsoleColor.Cyan;
            Console.WriteLine(valueAsString);

            var (result, childrenVariant) = client.Browse(address);
            if (result != DLR_RESULT.DL_OK)
            {
                return;
            }
            var children = childrenVariant.ToStringArray();
            if (children == null)
            {
                return;
            };

            for (var i = 0; i < children.Length; i++)
            {
                var rootAddress = string.IsNullOrEmpty(address) ? children[i] : $"{address}/{children[i]}";
                client.Traverse(rootAddress, children[i], indent, i == children.Length - 1);
            }

            Console.ResetColor();
        }

        /// <summary>
        /// Gets the value as string of the given address.
        /// </summary>
        /// <param name="client">The client.</param>
        /// <param name="address">The address.</param>
        /// <returns>The value as string.</returns>
        private static string GetValue(this IClient client, string address)
        {
            var (result, valueVariant) = client.ReadJson(address, -1);
            if (result == DLR_RESULT.DL_INVALID_ADDRESS)
            {
                return string.Empty;
            }

            if (result != DLR_RESULT.DL_OK)
            {
                return $"({result})";
            }

            if (valueVariant.IsNull)
            {
                return "null";
            }

            return Truncate(valueVariant.Value.ToString(), MaxChars);
        }

        /// <summary>
        /// Truncates the given string to a maximum count of characters defined by maxChars and append '...'
        /// </summary>
        /// <param name="value">The value<see cref="string"/>.</param>
        /// <param name="maxChars">The maxChars<see cref="int"/>.</param>
        /// <returns>The <see cref="string"/>.</returns>
        private static string Truncate(string value, int maxChars)
        {
            return value.Length <= maxChars ? value : value.Substring(0, maxChars) + "...";
        }
    }
}
