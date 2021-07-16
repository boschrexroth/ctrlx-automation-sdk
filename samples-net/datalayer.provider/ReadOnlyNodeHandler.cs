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

using Datalayer;
using System;

namespace Samples.Datalayer.Provider
{
    internal class ReadOnlyNodeHandler : IProviderNodeHandler
    {
        public ReadOnlyNodeHandler(IVariant value)
        {
            Value = value;
        }

        public IVariant Value { get; set; }

        public void OnCreate(string address, IVariant args, IProviderNodeResult result)
        {
            Console.WriteLine($"OnCreate {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        public void OnRemove(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnRemove {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        public void OnBrowse(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnBrowse {address}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        public void OnRead(string address, IVariant args, IProviderNodeResult result)
        {
            Console.WriteLine($"OnRead {address}: {args}");
            result.SetResult(DLR_RESULT.DL_OK, Value);
        }

        public void OnWrite(string address, IVariant writeValue, IProviderNodeResult result)
        {
            Console.WriteLine($"OnWrite {address}: {writeValue}");
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        public void OnMetadata(string address, IProviderNodeResult result)
        {
            Console.WriteLine($"OnMetadata {address}");

            if (!address.Equals(Program.AddressMyFlatbuffers))
            {
                result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
                return;
            }

            result.SetResult(DLR_RESULT.DL_OK, MetadataProvider.GetInstance());
        }
    }
}
