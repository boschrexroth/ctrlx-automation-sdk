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
    using System;

    /// <summary>
    /// ReadOnlyNodeHandler provides handler with read-only support.
    /// </summary>
    internal class ReadOnlyNodeHandler : IProviderNodeHandler
    {
        /// <summary>
        /// Gets the Node.
        /// </summary>
        public Node Node { get; }

        /// <summary>
        /// Initializes a new instance of the <see cref="ReadOnlyNodeHandler"/> class.
        /// </summary>
        /// <param name="node">The node<see cref="Node"/>.</param>
        public ReadOnlyNodeHandler(Node node)
        {
            Node = node;
        }

        /// <summary>
        /// The OnCreate callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="args">The args<see cref="IVariant"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnCreate(string address, IVariant args, IProviderNodeResult result)
        {
            Console.WriteLine($"OnCreate {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// The OnRemove callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnRemove(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnRemove {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// The OnBrowse callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnBrowse(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnBrowse {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// The OnRead callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="args">The args<see cref="IVariant"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnRead(string address, IVariant args, IProviderNodeResult result)
        {
            Console.WriteLine($"OnRead {address}: {args}");
            result.SetResult(DLR_RESULT.DL_OK, Node.Value);
        }

        /// <summary>
        /// The OnWrite callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="writeValue">The writeValue<see cref="IVariant"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnWrite(string address, IVariant writeValue, IProviderNodeResult result)
        {
            Console.WriteLine($"OnWrite {address}: {writeValue}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// The OnMetadata callback handler.
        /// </summary>
        /// <param name="address">The address<see cref="string"/>.</param>
        /// <param name="result">The result<see cref="IProviderNodeResult"/>.</param>
        public void OnMetadata(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnMetadata {address}");
            result.SetResult(DLR_RESULT.DL_OK, Node.Metadata);
        }
    }
}
