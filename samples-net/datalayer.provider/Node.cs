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
