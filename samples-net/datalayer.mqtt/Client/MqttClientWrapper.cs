
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
using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Client.Connecting;
using MQTTnet.Client.Disconnecting;
using MQTTnet.Client.Options;
using MQTTnet.Client.Publishing;
using MQTTnet.Client.Subscribing;
using MQTTnet.Client.Unsubscribing;
using MQTTnet.Exceptions;
using MQTTnet.Formatter;
using MQTTnet.Protocol;
using System;
using System.Net;
using System.Threading.Tasks;

namespace Samples.Datalayer.MQTT.Client
{
    /// <summary>
    /// Wrapper for MQTT Client (Protocol Version 5) based on MQTT.net
    /// https://github.com/chkr1011/MQTTnet
    /// https://github.com/andy840119/MQTTnet.Sample
    /// </summary>
    internal sealed class MqttClientWrapper
    {
        //Fields
        private readonly IMqttClient _client;

        //Events

        /// <summary>
        /// Message Received event
        /// </summary>
        public event Action<MqttApplicationMessageReceivedEventArgs> MessageReceived;

        /// <summary>
        /// Connected event
        /// </summary>
        public event Action<MqttClientConnectedEventArgs> Connected;

        /// <summary>
        /// Disconnected event
        /// </summary>
        public event Action<MqttClientDisconnectedEventArgs> Disconnected;

        /// <summary>
        /// Creates a MQTT client wrapper
        /// </summary>
        public MqttClientWrapper()
        {
            //Create client
            _client = new MqttFactory().CreateMqttClient();

            //Add handlers
            _client.UseConnectedHandler(OnConnected);
            _client.UseDisconnectedHandler(OnDisconnected);
            _client.UseApplicationMessageReceivedHandler(OnMessageReceived);
        }

        #region Properties

        /// <summary>
        /// Gets or sets the Server Address
        /// </summary>
        public string ServerAddress { get; set; } = "192.168.1.200";

        /// <summary>
        /// Gets or sets the Port
        /// </summary>
        public int Port { get; set; } = 1883;

        /// <summary>
        /// Sets or sets the ClientId
        /// </summary>
        public string ClientId { get; set; } = "";

        /// <summary>
        /// Gets or sets the Username
        /// </summary>
        public string Username { get; set; } = "";

        /// <summary>
        /// Gets or sets the Password
        /// </summary>
        public string Password { get; set; } = "";

        /// <summary>
        /// Gets or sets the communication timeout
        /// </summary>
        public TimeSpan CommunicationTimeout { get; set; } = TimeSpan.FromMilliseconds(10_000);

        /// <summary>
        /// 
        /// </summary>
        public TimeSpan KeepAlivePeriod { get; set; } = TimeSpan.FromMilliseconds(10_000);

        /// <summary>
        /// Gets or sets clean session
        /// </summary>
        public bool CleanSession { get; set; } = false;

        /// <summary>
        /// Indicates whether the client is connected
        /// </summary>
        public bool IsConnected => _client.IsConnected;

        #endregion

        #region Public

        /// <summary>
        /// Connects to a broker
        /// </summary>
        /// <returns></returns>
        public async Task<DLR_RESULT> ConnectAsync()
        {
            //Check if already connected
            if (IsConnected)
            {
                return DLR_RESULT.DL_OK;
            }

            //ClientId is mandatory
            if (string.IsNullOrWhiteSpace(ClientId))
            {
                throw new ArgumentException("no client id provided", nameof(ClientId));
            }

            try
            {
                Console.WriteLine($"Connecting to MQTT Broker: {ServerAddress}, ClientId: {ClientId}, CleanSession: {CleanSession} ...");

                //Run a MQTT publish client
                var task = _client.ConnectAsync(new MqttClientOptionsBuilder()
                    .WithTcpServer(ServerAddress, Port)
                    .WithClientId(ClientId)
                    .WithProtocolVersion(MqttProtocolVersion.V500) //Version 5 is Mandatory, because we make use of the SubscriptionIdentifier feature
                    .WithCleanSession(CleanSession)
                    .WithCredentials(username: Username, password: Password)
                    .WithCommunicationTimeout(CommunicationTimeout)
                    .WithKeepAlivePeriod(KeepAlivePeriod)
                    //.WithSessionExpiryInterval(60000)
                    //.WithTls()
                    .WithNoKeepAlive()
                    .Build()); ;

                var result = await task;

                //Failed
                if (result.ResultCode != MqttClientConnectResultCode.Success)
                {
                    Console.WriteLine($"Failed to connect to MQTT Broker: {ServerAddress}, Result: {result.ResultCode}");
                    return DLR_RESULT.DL_FAILED;
                }

                //Success
                Console.WriteLine($"Connected to MQTT Broker: {ServerAddress}");
                return DLR_RESULT.DL_OK;
            }
            catch (OperationCanceledException exc)
            {
                Console.WriteLine(exc.Message);
                return DLR_RESULT.DL_FAILED;
            }
            catch (MqttCommunicationException exc)
            {
                Console.WriteLine(exc.Message);
                return DLR_RESULT.DL_FAILED;
            }
        }

        /// <summary>
        /// Disconnects from a broker
        /// </summary>
        /// <returns></returns>
        public async Task<DLR_RESULT> DisconnectAsync()
        {
            //Check if already disconnected
            if (!IsConnected)
            {
                return DLR_RESULT.DL_OK;
            }

            var options = new MqttClientDisconnectOptions
            {
                ReasonCode = MqttClientDisconnectReason.NormalDisconnection,
                ReasonString = "normal disconnection"
            };

            try
            {
                Console.WriteLine($"Disconnecting from MQTT Broker: {ServerAddress} ...");

                var task = _client.DisconnectAsync(options);
                await task;

                //Failed
                if (!task.IsCompletedSuccessfully)
                {
                    Console.WriteLine($"Failed to disconnect from MQTT broker.");
                    return DLR_RESULT.DL_FAILED;
                }

                //Success
                Console.WriteLine($"Disconnected from MQTT broker.");
                return DLR_RESULT.DL_OK;
            }
            catch (MqttCommunicationException exc)
            {
                Console.WriteLine(exc.Message);
                return DLR_RESULT.DL_FAILED;
            }
        }

        /// <summary>
        ///  Publishes a message to a topic
        /// </summary>
        /// <param name="topic"></param>
        /// <param name="payload"></param>
        /// <param name="quality"></param>
        /// <param name="messageExpiryIntervalMillis"></param>
        /// <param name="retain"></param>
        /// <returns></returns>
        public async Task<DLR_RESULT> PublishAsync(
            string topic,
            string payload,
            MqttQualityOfServiceLevel quality = MqttQualityOfServiceLevel.AtLeastOnce,
            uint messageExpiryIntervalMillis = 0,
            bool retain = false)
        {
            if (string.IsNullOrWhiteSpace(topic))
            {
                throw new ArgumentException(nameof(topic));
            }

            //Check if connected
            if (!IsConnected)
            {
                Console.WriteLine($"Publish failed: '{DLR_RESULT.DL_CLIENT_NOT_CONNECTED}'");
                return DLR_RESULT.DL_CLIENT_NOT_CONNECTED;
            }

            var msg = new MqttApplicationMessageBuilder()
                .WithTopic(topic)
                .WithPayload(payload)
                .WithPayloadFormatIndicator(MqttPayloadFormatIndicator.Unspecified)
                .WithQualityOfServiceLevel(quality)
                .WithPayloadFormatIndicator(MqttPayloadFormatIndicator.CharacterData)
                .WithMessageExpiryInterval(messageExpiryIntervalMillis * 1000) // [ms] -> [s]
                .WithRetainFlag(retain).Build();

            try
            {
                var task = _client.PublishAsync(msg);
                await task;

                //Failed
                if (task.Result.ReasonCode != MqttClientPublishReasonCode.Success)
                {
                    Console.WriteLine($"Publish failed: '{task.Result.ReasonCode}'");
                    return DLR_RESULT.DL_FAILED;
                }

                //Success
                return DLR_RESULT.DL_OK;
            }
            catch (MqttCommunicationException exc)
            {
                Console.WriteLine(exc.Message);
                return DLR_RESULT.DL_FAILED;
            }
        }

        /// <summary>
        /// Subscribes a topic 
        /// </summary>
        /// <param name="topic"></param>
        /// <param name="subscriptionId"></param>
        /// <param name="quality"></param>
        /// <returns></returns>
        public async Task<DLR_RESULT> SubscribeAsync(
            string topic,
            uint subscriptionId,
            MqttQualityOfServiceLevel quality = MqttQualityOfServiceLevel.AtLeastOnce)
        {
            Console.WriteLine($"Subscribe topic: {topic}");

            if (string.IsNullOrWhiteSpace(topic))
            {
                throw new ArgumentException(nameof(topic));
            }

            if (subscriptionId == 0)
            {
                throw new ArgumentException("value must be greater than zero", nameof(subscriptionId));
            }

            //Check if connected
            if (!IsConnected)
            {
                Console.WriteLine($"Subscribe failed: {DLR_RESULT.DL_CLIENT_NOT_CONNECTED}");
                return DLR_RESULT.DL_CLIENT_NOT_CONNECTED;
            }

            var topicFilter = new MqttTopicFilterBuilder()
                .WithTopic(topic)
                .WithQualityOfServiceLevel(quality)
                .Build();

            var options = new MqttClientSubscribeOptionsBuilder()
                .WithTopicFilter(topicFilter)
                .WithSubscriptionIdentifier(subscriptionId)
                .Build();

            try
            {
                var task = _client.SubscribeAsync(options);
                await task;

                var resultCode = task.Result.Items[0].ResultCode;

                var success =
                    (quality == MqttQualityOfServiceLevel.AtMostOnce && resultCode == MqttClientSubscribeResultCode.GrantedQoS0) ||
                    (quality == MqttQualityOfServiceLevel.AtLeastOnce && resultCode == MqttClientSubscribeResultCode.GrantedQoS1) ||
                    (quality == MqttQualityOfServiceLevel.ExactlyOnce && resultCode == MqttClientSubscribeResultCode.GrantedQoS2);

                //Failed
                if (!success)
                {
                    Console.WriteLine($"Subscribe failed: {resultCode}");
                    return DLR_RESULT.DL_FAILED;
                }

                //Success
                return DLR_RESULT.DL_OK;
            }
            catch (MqttCommunicationException exc)
            {
                Console.WriteLine(exc.Message);
                return DLR_RESULT.DL_FAILED;
            }
        }

        /// <summary>
        /// Unsubscribes a topic
        /// </summary>
        /// <param name="topic"></param>
        /// <returns></returns>
        public async Task<DLR_RESULT> UnsubscribeAsync(string topic)
        {
            if (string.IsNullOrWhiteSpace(topic))
            {
                throw new ArgumentException(nameof(topic));
            }

            Console.WriteLine($"Unsubscribe topic: {topic}");

            //Check if connected
            if (!IsConnected)
            {
                Console.WriteLine($"Unsubscribe failed: {DLR_RESULT.DL_CLIENT_NOT_CONNECTED}");
                return DLR_RESULT.DL_CLIENT_NOT_CONNECTED;
            }

            var topicFilter = new MqttTopicFilterBuilder()
                .WithTopic(topic)
                .Build();

            var options = new MqttClientUnsubscribeOptionsBuilder()
                .WithTopicFilter(topicFilter)
                .Build();

            try
            {
                var task = _client.UnsubscribeAsync(options);
                var result = await task;

                var resultCode = result.Items[0].ReasonCode;

                //Failed
                if (resultCode != MqttClientUnsubscribeResultCode.Success)
                {
                    Console.WriteLine($"Unsubscribe failed: {resultCode}");
                    return DLR_RESULT.DL_FAILED;
                }

                //Success
                return DLR_RESULT.DL_OK;
            }
            catch (MqttCommunicationException exc)
            {
                Console.WriteLine(exc.Message);
                return DLR_RESULT.DL_FAILED;
            }
        }

        #endregion

        #region Public Static

        /// <summary>
        /// <param name="qos"></param>
        /// <returns></returns>
        /// Checks whether a given QoS level integer value is valid
        /// </summary>
        public static bool IsValidQualityOfServiceLevel(int qos)
        {
            return Enum.IsDefined(typeof(MqttQualityOfServiceLevel), qos);
        }

        /// <summary>
        /// Checks whether a given protocol version integer value is valid
        /// </summary>
        /// <param name="protocolVersion"></param>
        /// <returns></returns>
        public static bool IsValidProtocolVersion(int protocolVersion)
        {
            return Enum.IsDefined(typeof(MqttProtocolVersion), protocolVersion);
        }

        /// <summary>
        /// Returns a default client id
        /// </summary>
        /// <returns></returns>
        public static string DefaultClientId() => Dns.GetHostName() + "_" + DateTime.Now.Ticks.ToString();

        /// <summary>
        /// Returns a default topic
        /// </summary>
        public static string DefaultTopic => Dns.GetHostName();

        #endregion

        #region MQTT Event Handler

        /// <summary>
        /// Client connected event handler
        /// </summary>
        /// <param name="args"></param>
        private void OnConnected(MqttClientConnectedEventArgs args)
        {
            Console.WriteLine($"Client Connected. {args.AuthenticateResult?.ResultCode}");

            //Forward
            Connected?.Invoke(args);
        }

        /// <summary>
        /// Client disconnected event handler
        /// </summary>
        /// <param name="args"></param>
        private void OnDisconnected(MqttClientDisconnectedEventArgs args)
        {
            Console.WriteLine($"Client Disconnected. WasConnected: {args.ClientWasConnected}, Reason: {args.Reason}");

            //Forward
            Disconnected?.Invoke(args);
        }

        /// <summary>
        /// Client message received eventhandler
        /// </summary>
        /// <param name="args"></param>
        private void OnMessageReceived(MqttApplicationMessageReceivedEventArgs args)
        {
            //Console.WriteLine($"Message Received: {args.ApplicationMessage.Topic}, PayLoad: {Encoding.UTF8.GetString(args.ApplicationMessage.Payload)}, SubscriptionId: {args.ApplicationMessage.SubscriptionIdentifiers?[0]}");

            //Forward
            MessageReceived?.Invoke(args);
        }
        #endregion
    }
}
