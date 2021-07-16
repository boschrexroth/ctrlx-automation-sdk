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
using Samples.Datalayer.MQTT.Base;

namespace Samples.Datalayer.MQTT.Test
{
    /// <summary>
    /// Handler for test node
    /// </summary>
    internal class MqttTestNodeHandler : MqttBaseNodeHandler
    {
        /// <summary>
        /// Creates the handler
        /// </summary>
        /// <param name="root"></param>
        /// <param name="parent"></param>
        public MqttTestNodeHandler(MqttRootNodeHandler root, MqttBaseNodeHandler parent) :
            base(root, parent, "test")
        {
        }

        #region Public

        /// <summary>
        /// Returns the Dummy1 node address
        /// </summary>
        public string Dummy1 => $"{FullAddress}/{Names.Dummy1}";

        /// <summary>
        /// Returns the Dummy2 node address
        /// </summary>
        public string Dummy2 => $"{FullAddress}/{Names.Dummy2}";

        /// <summary>
        /// Returns the Dummy3 node address
        /// </summary>
        public string Dummy3 => $"{FullAddress}/{Names.Dummy3}";

        #endregion

        #region Overrides

        /// <summary>
        /// Starts the handler
        /// </summary>
        /// <returns></returns>
        public override DLR_RESULT Start()
        {
            //dummy1
            var (result, node) = Root.Provider.CreateNode(this, FullAddress, Names.Dummy1, Variant.Null);
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            //dummy2
            (result, node) = Root.Provider.CreateNode(this, FullAddress, Names.Dummy2, Variant.Null);
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            //dummy3
            (result, node) = Root.Provider.CreateNode(this, FullAddress, Names.Dummy3, Variant.Null);
            if (result.IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }
            Nodes.Add(node.Address, node);

            return base.Start();
        }

        /// <summary>
        /// OnWrite handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="writeValue"></param>
        /// <param name="result"></param>
        public override void OnWrite(string address, IVariant writeValue, IProviderNodeResult result)
        {
            //Fetch the node
            if (!Nodes.TryGetValue(address, out ProviderNodeWrapper wrappedNode))
            {
                result.SetResult(DLR_RESULT.DL_FAILED);
                return;
            }

            wrappedNode.Value = writeValue;
            result.SetResult(DLR_RESULT.DL_OK, wrappedNode.Value);
        }

        #endregion
    }
}
