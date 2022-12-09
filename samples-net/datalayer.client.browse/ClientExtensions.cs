/*
MIT License

Copyright (c) 2021-2022 Bosch Rexroth AG

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
        private static readonly int MaxChars = 100;

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
            if (result != DLR_RESULT.DL_OK) return;
            var children = childrenVariant.ToStringArray();
            if (children == null) return;

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
