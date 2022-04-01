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
using FlatBuffers;
using System;
using System.Collections.Generic;
using System.Text;

namespace Samples.Datalayer.MQTT.Base
{
    /// <summary>
    /// Base node handler we can derive from
    /// </summary>
    internal abstract class MqttBaseNodeHandler : IProviderNodeHandler
    {
        //Consts
        protected const uint TimeoutMillis = 10_000;

        /// <summary>
        /// Creates a top level node handler
        /// </summary>
        /// <param name="baseAddress"></param>
        /// <param name="name"></param>
        /// <param name="value"></param>
        public MqttBaseNodeHandler(string baseAddress, string name)
        {
            BaseAddress = baseAddress ?? throw new ArgumentNullException(nameof(baseAddress));
            Name = name ?? throw new ArgumentNullException(nameof(name));

            FullAddress = $"{BaseAddress}/{Name}";
        }

        /// <summary>
        /// Creates the child node handler with a root and parent
        /// </summary>
        /// <param name="root"></param>
        /// <param name="parent"></param>
        /// <param name="name"></param>
        public MqttBaseNodeHandler(MqttRootNodeHandler root, MqttBaseNodeHandler parent, string name)
        {
            Root = root ?? throw new ArgumentNullException(nameof(root));
            Parent = parent ?? throw new ArgumentNullException(nameof(parent));
            Name = name ?? throw new ArgumentNullException(nameof(name));

            BaseAddress = parent.FullAddress;
            FullAddress = $"{BaseAddress}/{Name}";
        }

        #region Properties

        /// <summary>
        /// Gets the name
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// Gets the base address
        /// </summary>
        public string BaseAddress { get; }

        /// <summary>
        /// Gets the full address with is BaseAddress/Name
        /// </summary>
        public string FullAddress { get; }

        /// <summary>
        /// Gets the list of child handlers
        /// </summary>
        /// 
        public List<MqttBaseNodeHandler> Handlers { get; } = new();

        /// <summary>
        /// Gets the list of Nodes
        /// </summary>
        public Dictionary<string, ProviderNodeWrapper> Nodes { get; } = new();

        /// <summary>
        /// Gets the parent node handler
        /// </summary>
        public MqttBaseNodeHandler Parent { get; }

        /// <summary>
        /// Gets the root node handler
        /// </summary>
        public MqttRootNodeHandler Root { get; }

        /// <summary>
        /// Returns whether the handler is beeing stopped
        /// </summary>
        public bool IsShutdown { get; private set; }

        /// <summary>
        /// Returns whether the handler is started
        /// </summary>
        public bool IsStarted { get; private set; }

        #endregion

        #region Public

        /// <summary>
        /// Indicates whether a node present in list of handled nodes
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public bool HasNode(string name)
        {
            return Nodes.ContainsKey($"{FullAddress}/{name}");
        }

        /// <summary>
        /// Gets the wrapped node by name searching list of handled nodes
        /// </summary>
        /// <exception cref="KeyNotFoundException">If node not present. Use HasNode() for safe check if necessary</exception> 
        /// <param name="name"></param>
        /// <returns></returns>
        public ProviderNodeWrapper GetNode(string name)
        {
            return Nodes[$"{FullAddress}/{name}"];
        }

        #endregion

        #region Public Static

        /// <summary>
        /// Indicates whether the application is snapped or not (running inside linux snappy environment)
        /// </summary>
        /// <returns></returns>
        public static bool IsSnapped => !string.IsNullOrEmpty(Environment.GetEnvironmentVariable("SNAP"));

        /// <summary>
        /// Returns a Variant containing the stringified result as value
        /// </summary>
        /// <param name="result"></param>
        /// <returns></returns>
        public static IVariant ToStatus(DLR_RESULT result)
        {
            return new Variant(result.ToString());
        }

        /// <summary>
        /// Returns the best machting Variant
        /// </summary>
        /// <param name="stringifiedValue"></param>
        /// <returns></returns>
        public static IVariant ToVariant(string stringifiedValue)
        {
            //Check for empty value -> no value (null)
            if (stringifiedValue.Length == 0)
            {
                return Variant.Null;
            }

            //Check for any numeric value
            if (char.IsDigit(stringifiedValue[0]))
            {
                //Check for double value 
                if (stringifiedValue.IndexOf(".") > 0)
                {
                    if (double.TryParse(stringifiedValue, out var doubleResult))
                    {
                        return new Variant(doubleResult);
                    }
                }

                //Check for int value 
                if (int.TryParse(stringifiedValue, out var intResult))
                {
                    return new Variant(intResult);
                }
            }

            //Check for boolean value
            if (bool.TryParse(stringifiedValue, out var boolResult))
            {
                return new Variant(boolResult);
            }

            //Arrays: NOT SUPPORTED YET

            //Default -> String
            return new Variant(stringifiedValue);
        }

        /// <summary>
        /// Converts a stringified variant value into it's Variant holding a value of representing json data type
        /// </summary>
        /// <param name="jsonDataType"></param>
        /// <param name="stringifiedValue"></param>
        /// <returns></returns>
        public static (DLR_RESULT, Variant) ToVariant(string jsonDataType, string stringifiedValue)
        {
            try
            {
                return jsonDataType switch
                {
                    "bool8" => (DLR_RESULT.DL_OK, new Variant(Convert.ToBoolean(stringifiedValue))),
                    "int8" => (DLR_RESULT.DL_OK, new Variant(Convert.ToSByte(stringifiedValue))),
                    "uint8" => (DLR_RESULT.DL_OK, new Variant(Convert.ToByte(stringifiedValue))),
                    "int16" => (DLR_RESULT.DL_OK, new Variant(Convert.ToInt16(stringifiedValue))),
                    "uint16" => (DLR_RESULT.DL_OK, new Variant(Convert.ToUInt16(stringifiedValue))),
                    "int32" => (DLR_RESULT.DL_OK, new Variant(Convert.ToInt32(stringifiedValue))),
                    "uint32" => (DLR_RESULT.DL_OK, new Variant(Convert.ToUInt32(stringifiedValue))),
                    "int64" => (DLR_RESULT.DL_OK, new Variant(Convert.ToInt64(stringifiedValue))),
                    "uint64" => (DLR_RESULT.DL_OK, new Variant(Convert.ToUInt64(stringifiedValue))),
                    "float" => (DLR_RESULT.DL_OK, new Variant(Convert.ToSingle(stringifiedValue))),
                    "double" => (DLR_RESULT.DL_OK, new Variant(Convert.ToDouble(stringifiedValue))),
                    "string" => (DLR_RESULT.DL_OK, new Variant(stringifiedValue)),

                    "raw" => (DLR_RESULT.DL_OK, new Variant(Encoding.UTF8.GetBytes(stringifiedValue))),
                    "flatbuffers" => (DLR_RESULT.DL_OK, new Variant(Encoding.UTF8.GetBytes(stringifiedValue))),

                    //Arrays: NOT SUPPORTED YET
                    "arbool8" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    "arint8" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    "aruint8" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    "arint16" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    "aruint16" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    "arint32" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    "aruint32" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    "arint64" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    "aruint64" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    "arfloat" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    "ardouble" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    "arstring" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),

                    "unknown" => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                    _ => (DLR_RESULT.DL_UNSUPPORTED, Variant.Null),
                };
            }
            catch (OverflowException exc)
            {
                Console.WriteLine(exc.Message);
                return (DLR_RESULT.DL_INVALID_VALUE, Variant.Null);
            }
            catch (FormatException exc)
            {
                Console.WriteLine(exc.Message);
                return (DLR_RESULT.DL_TYPE_MISMATCH, Variant.Null);
            }
        }

        #endregion

        #region Virtual / Event Handler

        /// <summary>
        /// Starts the handler recursively
        /// </summary>
        /// <returns></returns>
        public virtual DLR_RESULT Start()
        {
            if (Handlers == null)
            {
                return DLR_RESULT.DL_OK;
            }

            foreach (var child in Handlers)
            {
                if (child.Start().IsBad())
                {
                    return DLR_RESULT.DL_FAILED;
                }
            }

            IsStarted = true;
            return DLR_RESULT.DL_OK;
        }

        /// <summary>
        /// Stops the handler and unregisters the nodes recursively
        /// </summary>
        /// <returns></returns>
        public virtual DLR_RESULT Stop()
        {
            //First, we set the shutdown flag
            IsShutdown = true;

            //Unregister Nodes
            UnregisterNodes();

            if (Handlers == null)
            {
                return DLR_RESULT.DL_OK;
            }

            foreach (var child in Handlers)
            {
                if (child.Stop().IsBad())
                {
                    return DLR_RESULT.DL_FAILED;
                }
            }

            return DLR_RESULT.DL_OK;
        }

        /// <summary>
        /// OnCreate handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="args"></param>
        /// <param name="result"></param>
        public virtual void OnCreate(string address, IVariant args, IProviderNodeResult result)
        {
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// OnRemove handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public virtual void OnRemove(string address, IProviderNodeResult result)
        {
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
        }

        /// <summary>
        /// OnRead handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="args"></param>
        /// <param name="result"></param>
        public virtual void OnRead(string address, IVariant args, IProviderNodeResult result)
        {
            //Fetch the node
            if (!Nodes.TryGetValue(address, out ProviderNodeWrapper wrappedNode))
            {
                result.SetResult(DLR_RESULT.DL_FAILED);
                return;
            }

            result.SetResult(DLR_RESULT.DL_OK, wrappedNode.Value);
        }

        /// <summary>
        /// OnWrite handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="writeValue"></param>
        /// <param name="result"></param>
        public virtual void OnWrite(string address, IVariant writeValue, IProviderNodeResult result)
        {
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED); //Default
        }

        /// <summary>
        /// OnBrowse handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public virtual void OnBrowse(string address, IProviderNodeResult result)
        {
            result.SetResult(DLR_RESULT.DL_UNSUPPORTED); //Default
        }

        /// <summary>
        /// OnMetadata handler
        /// </summary>
        /// <param name="address"></param>
        /// <param name="result"></param>
        public virtual void OnMetadata(string address, IProviderNodeResult result)
        {
            //Fetch the node
            if (!Nodes.TryGetValue(address, out ProviderNodeWrapper wrappedNode))
            {
                result.SetResult(DLR_RESULT.DL_UNSUPPORTED);
                return;
            }

            CreateMetadata(wrappedNode, result);
        }

        #endregion

        #region Protected

        /// <summary>
        /// Unregister the Nodes
        /// </summary>
        /// <returns></returns>
        protected DLR_RESULT UnregisterNodes()
        {
            //Register all nodes by it's address
            foreach (var address in Nodes.Keys)
            {
                if (Root.Provider.UnregisterNode(address).IsBad())
                {
                    return DLR_RESULT.DL_FAILED;
                }
            }

            Nodes.Clear();
            return DLR_RESULT.DL_OK;
        }

        #endregion

        #region Private

        /// <summary>
        /// Creates meta informations for a wrapped node
        /// </summary>
        /// <param name="wrappedNode"></param>
        /// <param name="result"></param>
        private static void CreateMetadata(ProviderNodeWrapper wrappedNode, IProviderNodeResult result)
        {
            //Create MetaData
            var builder = new FlatBufferBuilder(Variant.DefaultFlatbuffersInitialSize);

            var operations = AllowedOperations.CreateAllowedOperations(builder,
                read: wrappedNode.IsReadable,
                write: wrappedNode.IsWriteable,
                browse: wrappedNode.IsBrowsable,
                create: wrappedNode.IsCreatable,
                delete: wrappedNode.IsDeleteable);

            var metaData = Metadata.CreateMetadata(builder,
                nodeClass: wrappedNode.Category,
                operationsOffset: operations,
                descriptionOffset: builder.CreateString(wrappedNode.Address),
                descriptionUrlOffset: builder.CreateString(wrappedNode.Address));

            builder.Finish(metaData.Value);
            result.SetResult(DLR_RESULT.DL_OK, new Variant(builder));
        }

        #endregion
    }
}
