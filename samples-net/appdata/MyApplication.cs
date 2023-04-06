/*
MIT License

Copyright (c) 2021-2023 Bosch Rexroth AG

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
using Microsoft.IdentityModel.JsonWebTokens;
using System;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;

namespace Samples.Appdata
{
    /// <summary>
    /// 
    /// </summary>
    internal class MyApplication
    {
        //This samples uses a HTTP Listener as light-weight HTTP Server
        //https://docs.microsoft.com/en-us/dotnet/api/system.net.httplistener?view=net-5.0
        //An alternative solution would be use of 'Unix Domain Sockets' instead of an HTTP server, which is more safe, but would result in more complicated HTTP request/result handling here.
        //For those scenarious ASP.NET comes in place with a rich built-in web server supporting binding on unix sockets.

        //Please see application manifest file ./configs/ctrlx-dotnet-appdata.package-manifest.json for details.

        //Setup HTTP listener and load/save routes we're interested in
        private const int HttpPort = 5555; //We have to choose a free port we're listening on
        private static readonly string HttpApiRouteLoad = $"http://localhost:{HttpPort}/ctrlx-dotnet-appdata/api/v1/load";
        private static readonly string HttpApiRouteSave = $"http://localhost:{HttpPort}/ctrlx-dotnet-appdata/api/v1/save";

        /// The name of the storage file
        private const string StorageFileName = "appdata.json";

        /// The name of the application storage folder
        /// MUST be same as specified in your *.package-manifest.json file
        private const string StorageFolderName = "appdatasample";

        //Fields
        private HttpListener _httpListener;

        // Represents an application data HTTP request command 
        private record AppDataHttpRequest(string ConfigurationPath, string Id, string Phase);

        // Represents our application data to be loaded/saved as JSON
        private record AppData(string HostName, bool IsLinux, string OSArchitecture, DateTime TimeStamp, int SecretNumber);

        #region Properties

        /// <summary>
        /// Gets the application lock
        /// </summary>
        public ManualResetEvent Lock { get; } = new(false);

        /// <summary>
        /// Gets and sets the application data
        /// </summary>
        private AppData MyAppData { get; set; }

        /// <summary>
        /// Indicates whether the application is snapped or not (running inside linux snappy environment)
        /// </summary>
        /// <returns></returns>
        private static bool IsSnapped => !string.IsNullOrEmpty(Environment.GetEnvironmentVariable("SNAP"));

        /// <summary>
        /// Gets the SNAP_COMMON location
        /// </summary>
        private static string SnapCommonLocation => Environment.GetEnvironmentVariable("SNAP_COMMON");

        /// <summary>
        /// Gets the SNAP_DATA location
        /// </summary>
        private static string SnapDataLocation => Environment.GetEnvironmentVariable("SNAP_DATA");

        /// <summary>
        /// Gets the base storage location for all applications
        /// 
        /// Snapped (Linux): $SNAP_COMMON/solutions/activeConfiguration
        /// Windows: C:\Users\{USER}\Documents\My ctrlX
        /// </summary>
        private static string BaseStorageLocation => IsSnapped ?
            Path.Combine(SnapCommonLocation, "solutions", "activeConfiguration") :
            Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "My ctrlX");

        /// <summary>
        /// Gets the application data storage location
        /// </summary>
        private static string StorageLocation => Path.Combine(BaseStorageLocation, StorageFolderName);

        /// <summary>
        /// Gets the application data storage file
        /// </summary>
        private static string StorageFile => Path.Combine(StorageLocation, StorageFileName);

        #endregion

        #region Public

        /// <summary>
        /// Starts the handler
        /// </summary>
        /// <returns></returns>
        public DLR_RESULT Start()
        {
            // Check if the process is running inside a snap 
            Console.WriteLine($"Running inside snap: {IsSnapped}");

            //Load the application data
            if (Load().IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }

            //Start the HTTP Listener
            if (StartListenHttp().IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }

            return DLR_RESULT.DL_OK;
        }

        /// <summary>
        /// Stops the application
        /// </summary>
        /// <returns></returns>
        public DLR_RESULT Stop()
        {
            //Save the application data (discard result)
            Save();

            //Stop HTTP listening (discard result)
            StopListenHttp();

            //Unlock application
            Lock.Set();

            return DLR_RESULT.DL_OK;
        }

        #endregion

        #region Private

        /// <summary>
        /// Resets the application data
        /// </summary>
        /// <returns></returns>
        private DLR_RESULT Reset()
        {
            //Create new application data
            MyAppData = new AppData(
                HostName: Dns.GetHostName(),
                IsLinux: RuntimeInformation.IsOSPlatform(OSPlatform.Linux),
                OSArchitecture: RuntimeInformation.OSArchitecture.ToString(),
                TimeStamp: DateTime.Now,
                SecretNumber: 4711);

            Console.WriteLine($"Created new application data.");

            //Save
            return Save();
        }

        /// <summary>
        /// Loads (deserializes) application data from a file (JSON)
        /// </summary>
        /// <returns></returns>
        private DLR_RESULT Load()
        {
            var path = StorageFile;

            if (!File.Exists(path))
            {
                return Reset();
            }

            try
            {
                var jsonString = File.ReadAllText(path, Encoding.UTF8);
                MyAppData = JsonSerializer.Deserialize<AppData>(jsonString);

                Console.WriteLine($"Loaded application data from file '{path}'.");
                return DLR_RESULT.DL_OK;
            }
            catch (Exception exc) when (exc is IOException || exc is JsonException)
            {
                Console.WriteLine($"Loading application data from file '{path}' failed! {exc.Message}");
                return DLR_RESULT.DL_FAILED;
            }
        }

        /// <summary>
        /// Saves (serializes) the application data into a file (JSON)
        /// </summary>
        /// <returns></returns>
        private DLR_RESULT Save()
        {
            //Ensure the storage location, first.
            if (EnsureStorageLocation().IsBad())
            {
                return DLR_RESULT.DL_FAILED;
            }

            var path = StorageFile;

            try
            {
                var options = new JsonSerializerOptions()
                {
                    WriteIndented = true
                };

                var jsonString = JsonSerializer.Serialize(MyAppData, options);
                File.WriteAllText(path, jsonString, Encoding.UTF8);

                Console.WriteLine($"Saved application data to file '{path}'.");
                return DLR_RESULT.DL_OK;
            }
            catch (IOException exc)
            {
                Console.WriteLine($"Saving application data to file '{path}' failed! {exc.Message}");
                return DLR_RESULT.DL_FAILED;
            }
        }

        /// <summary>
        /// Ensures the Storage Location
        /// </summary>
        /// <returns></returns>
        private static DLR_RESULT EnsureStorageLocation()
        {
            var path = StorageLocation;

            if (!Directory.Exists(path))
            {
                try
                {
                    Directory.CreateDirectory(path);
                    Console.WriteLine($"Created storage location: '{path}'.");
                }
                catch (IOException exc)
                {
                    Console.WriteLine($"Creating storage location '{path}' failed! {exc.Message}");
                    return DLR_RESULT.DL_FAILED;
                }
            }

            return DLR_RESULT.DL_OK;
        }

        /// <summary>
        /// Indicates whether the request is authorized to be executed
        /// </summary>
        /// <returns></returns>
        private static bool IsAuthorized(JsonWebToken jwt)
        {
            //Extract the scopes
            var scopes = jwt.GetPayloadValue<string[]>("scope");

            //Check permissions
            foreach (var scope in scopes)
            {
                //Add your additional scopes here on demand
                if (scope.Equals("rexroth-device.all.rwx"))
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// Gets the JWT Token
        /// </summary>
        /// <returns></returns>
        private static (DLR_RESULT, JsonWebToken) GetToken(HttpListenerRequest request)
        {
            //Extract the token
            var token = request.Headers["Authorization"].Split(" ").Last();

            //Optional validate the request here
#if (false)
            var tokenHandler = new JsonWebTokenHandler();
            var validationResult = tokenHandler.ValidateToken(token, new TokenValidationParameters
            {
                //Enable your checks here ...
                ValidateLifetime = true,
                ValidateIssuer = true,
                ValidateTokenReplay = true,
                ValidateIssuerSigningKey = true,
                ValidateActor = true,
                ValidateAudience = true,

                // set clockskew to zero so tokens expire exactly at token expiration time (instead of 5 minutes later)
                ClockSkew = TimeSpan.Zero
            });

            Console.WriteLine($"token valid: {validationResult.IsValid}");
            if (!validationResult.IsValid)
            {
                return (DLR_RESULT.DL_FAILED, null);
            }
#endif
            //Return the JWT
            return (DLR_RESULT.DL_OK, new JsonWebToken(token));
        }

        /// <summary>
        /// Starts the HTTP Listener
        /// </summary>
        /// <returns></returns>
        private DLR_RESULT StartListenHttp()
        {
            try
            {
                //Check if supported          
                if (!HttpListener.IsSupported)
                {
                    Console.WriteLine("HTTP Listening not supported!");
                    return DLR_RESULT.DL_UNSUPPORTED;
                }

                // Create a listener.
                _httpListener = new HttpListener();

                // Add the routes (prefixes); must be end with a slash
                _httpListener.Prefixes.Add(HttpApiRouteLoad + "/");
                _httpListener.Prefixes.Add(HttpApiRouteSave + "/");

                //Start listening
                _httpListener.Start();

                //Check if we're listening
                if (!_httpListener.IsListening)
                {
                    Console.WriteLine($"Listening to HTTP failed!");
                    return DLR_RESULT.DL_FAILED;
                }

                Console.WriteLine($"Listening to HTTP: {string.Join(", ", _httpListener.Prefixes.ToArray())}");

                //Listen
                Task.Factory.StartNew(() =>
                {
                    while (true)
                    {
                        // Note: The GetContext method blocks while waiting for a request.
                        var context = _httpListener.GetContext();
                        var request = context.Request;
                        var response = context.Response;

                        //We received a new app data REST command
                        Console.WriteLine($"Request: {request.Url}");

                        //Get the authentication token (Bearer)
                        var (result, jwt) = GetToken(request);
                        if (result.IsBad())
                        {
                            Console.WriteLine("Invalid authentication token (Bearer)!");
                            response.StatusCode = (int)HttpStatusCode.Unauthorized;
                            response.Close();
                            continue;
                        }

                        //Check if we can execute the request
                        if (!IsAuthorized(jwt))
                        {
                            Console.WriteLine("Unauthorized!");
                            response.StatusCode = (int)HttpStatusCode.Unauthorized;
                            response.Close();
                            continue;
                        }

                        try
                        {
                            //Deserialize Request
                            using var reader = new StreamReader(request.InputStream, request.ContentEncoding);

                            var appdataRequest = JsonSerializer.Deserialize<AppDataHttpRequest>(reader.ReadToEnd(), new JsonSerializerOptions
                            {
                                PropertyNameCaseInsensitive = true
                            });

                            Console.WriteLine($"Payload: {appdataRequest}");
                            var route = request.Url.ToString();

                            //Load Command
                            if (route.Equals(HttpApiRouteLoad))
                            {
                                //Phases
                                switch (appdataRequest.Phase)
                                {
                                    // Phases we don't care about in this sample can be implemented on demand.

                                    // query: Check if loading is possible in the current system statecase "query":
                                    // Hint: The phase 'query' is called 2 times: the first call asks for a setup mode change acknowledge and the second is called during normal load sequence
                                    case "query":
                                    // prepare: Perform any required preparatory steps
                                    case "prepare":
                                    // validate: Perform post-processing steps, connect resources and resolve dependencies
                                    case "validate":
                                    // activate (if phases 1-4 have been finished without problems): Establish desired run state of the device
                                    case "activate":
                                    // abort (otherwise): Do NOT change run state of the device, clean up if required
                                    case "abort":
                                        //We return 204 as an default to satisfy the workflow
                                        response.StatusCode = (int)HttpStatusCode.NoContent;
                                        break;

                                    // load: Provide resources according to the data from the active configuration
                                    case "load":
                                        //This is were we can load our application data from current configuration
                                        response.StatusCode = Load().IsGood() ? (int)HttpStatusCode.Accepted : (int)HttpStatusCode.InternalServerError;
                                        break;

                                    default:
                                        //We return 204 here to add upwards-compatibility for new phases
                                        response.StatusCode = (int)HttpStatusCode.NoContent;
                                        break;
                                }
                            }
                            //Save Command
                            else if (route.Equals(HttpApiRouteSave))
                            {
                                //Phases
                                switch (appdataRequest.Phase)
                                {
                                    //save: Serialize current resources into active configuration
                                    case "save":
                                        //This is were we can save our application data into current configuration to be persistent
                                        response.StatusCode = Save().IsGood() ? (int)HttpStatusCode.Accepted : (int)HttpStatusCode.InternalServerError;
                                        break;

                                    default:
                                        //We return 204 here to add upwards-compatibility for new phases
                                        response.StatusCode = (int)HttpStatusCode.NoContent;
                                        break;
                                }
                            }
                        }
                        catch (JsonException exc)
                        {
                            //IMPORTANT: 
                            //We have to handle _ALL_ possible exceptions here and _ALLWAYS_ return a response!              
                            Console.WriteLine($"Failed to parse appdata request! {exc.Message}");
                            response.StatusCode = (int)HttpStatusCode.InternalServerError;
                        }
                        response.Close();
                    }
                });
                return DLR_RESULT.DL_OK;
            }
            catch (HttpListenerException exc)
            {
                Console.WriteLine($"Listening to HTTP failed! {exc.Message}");
                return DLR_RESULT.DL_FAILED;
            }
        }

        /// <summary>
        /// Stops the HTTP Listener
        /// </summary>
        /// <returns></returns>
        private void StopListenHttp()
        {
            _httpListener.Stop();
        }

        #endregion
    }
}
