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
namespace Samples.Datalayer.MQTT.Base
{
    /// <summary>
    /// 
    /// </summary>
    internal static class Names
    {
        public const string Create = "» create";
        public const string Remove = "× remove";
        public const string Status = "• status";
        public const string Topic = "topic";
        public const string QualityOfService = "quality-of-service";

        public const string ServerAddress = "server-address";
        public const string Port = "port";
        public const string ClientId = "client-id";
        public const string Username = "username";
        public const string Password = "password";
        public const string CleanSession = "clean-session";
        public const string CommunicationTimeoutMillis = "communication-timeout-millis";
        public const string KeepAlivePeriodMillis = "keep-alive-period-millis";

        public const string SourceAddress = "source-address";
        public const string PublishIntervalMillis = "publish-interval-millis";
        public const string Retain = "retain";
        public const string MessageExpiryIntervalMillis = "message-expiry-inverval-millis";
        public const string TargetAddress = "target-address";
        public const string JsonDataType = "json-datatype";

        public const string Dummy1 = "dummy1";
        public const string Dummy2 = "dummy2";
        public const string Dummy3 = "dummy3";

        public const string ConfigPrefix = "config_";
    }
}
