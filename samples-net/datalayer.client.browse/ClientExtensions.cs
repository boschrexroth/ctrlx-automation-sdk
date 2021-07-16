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

using comm.datalayer;
using Datalayer;
using System;

namespace Samples.Datalayer.Client.Browse
{
    /// <summary>
    /// Provides extension methods for IClient
    /// </summary>
    internal static class ClientExtensions
    {
        /// <summary>
        /// Browses all nodes in given address and write leaf and value with indentation to console.
        /// </summary>
        /// <param name="client">The client</param>
        /// <param name="converter">The converter</param>
        /// <param name="address">The address</param>
        /// <param name="leaf">The leaf</param>
        /// <param name="indent">The indentation</param>
        /// <param name="last">The last</param>
        internal static void BrowseTree(this IClient client, IConverter converter, string address = "", string leaf = "", string indent = "",
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

            var valueAsString = client.GetValue(converter, address);

            Console.ForegroundColor = valueAsString.StartsWith("error:") ? ConsoleColor.Red : ConsoleColor.Cyan;
            Console.WriteLine(valueAsString);

            var (result, childrenVariant) = client.Browse(address);
            if (result != DLR_RESULT.DL_OK) return;
            var children = childrenVariant.ToStringArray();
            if (children == null) return;

            for (var i = 0; i < children.Length; i++)
            {
                var rootAddress = string.IsNullOrEmpty(address) ? children[i] : $"{address}/{children[i]}";
                client.BrowseTree(converter, rootAddress, children[i], indent, i == children.Length - 1);
            }
            Console.ResetColor();
        }

        /// <summary>
        /// Gets the value as string of the given address.
        /// </summary>
        /// <param name="client">The client</param>
        /// <param name="converter">The converter</param>
        /// <param name="address">The address</param>
        /// <returns>The value as string</returns>
        private static string GetValue(this IClient client, IConverter converter, string address)
        {
            var (result, valueVariant) = client.Read(address);
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

            if (valueVariant.IsArray)
            {
                var objectArray = valueVariant.Value as object[] ?? Array.Empty<object>();
                var stringArray = string.Join(",", objectArray);
                return $"[{stringArray}]";
            }

            if (valueVariant.IsFlatbuffers)
            {
                var typeAddress = client.GetTypeAddress(address);
                if (typeAddress == null)
                {
                    return "error: no read type found";
                }

                var (resultType, typeVariant) = client.Read(typeAddress);
                if (resultType != DLR_RESULT.DL_OK)
                {
                    return $"error: read type {typeAddress} failed";
                }

                var (resultJson, json) = converter.GenerateJsonComplex(valueVariant, typeVariant, -1);
                if (resultJson != DLR_RESULT.DL_OK)
                {
                    return $"error: generate json of type {typeAddress} failed";
                }

                return json.Value.ToString();
            }

            return valueVariant.Value.ToString();
        }

        /// <summary>
        /// Get the type address of the given address.
        /// </summary>
        /// <param name="client">The client</param>
        /// <param name="address">The address</param>
        /// <returns>The type address as string</returns>
        private static string GetTypeAddress(this IClient client, string address)
        {
            var (resultReadMetadata, metadataVariant) = client.ReadMetadata(address);
            if (resultReadMetadata != DLR_RESULT.DL_OK) return null;

            var metaData = Metadata.GetRootAsMetadata(metadataVariant.ToFlatbuffers());
            if (metaData.ReferencesLength == 0) return null;

            string readTypeAddress = null;
            for (var i = 0; i < metaData.ReferencesLength; i++)
            {
                var reference = metaData.References(i);
                if (!reference.HasValue)
                {
                    continue;
                }

                if (reference.Value.Type.ToLower() != "readtype")
                {
                    continue;
                }

                readTypeAddress = reference.Value.TargetAddress;
            }

            return readTypeAddress;
        }
    }
}
