/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

using Microsoft.IdentityModel.JsonWebTokens;
using System;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace Samples.AppData
{

    internal class MyApplication
    {
        //This samples uses a HTTP Listener as light-weight HTTP Server
        //https://docs.microsoft.com/en-us/dotnet/api/system.net.httplistener?view=net-5.0
        //An alternative solution would be use of 'Unix Domain Sockets' instead of an HTTP server, which is more safe, but would result in more complicated HTTP request/result handling here.
        //For those scenarious ASP.NET comes in place with a rich built-in web server supporting binding on unix sockets.

        //Please see application manifest file ./configs/sdk-net-appdata.package-manifest.json for details.

        //Setup HTTP listener and load/save routes we're interested in
        private const int HttpPort = 5555; //We have to choose a free port we're listening on
        private static readonly string HttpApiRouteLoad = $"http://localhost:{HttpPort}/sdk-net-appdata/api/v1/load";
        private static readonly string HttpApiRouteSave = $"http://localhost:{HttpPort}/sdk-net-appdata/api/v1/save";

        /// The name of the storage file
        private const string StorageFileName = "appdata.json";

        /// The name of the application storage folder
        /// MUST be same as specified in your *.package-manifest.json file
        private const string StorageFolderName = "sdk-net-appdata";

        //Fields
        private HttpListener _httpListener;

        // Represents an application data HTTP request command 
        private record AppDataHttpRequest(string ConfigurationPath, string Id, string Phase);

        // Represents our application data to be loaded/saved as JSON
        private record AppData(string HostName, bool IsLinux, string OsArchitecture, DateTime TimeStamp, int SecretNumber);

        #region Properties

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
        public bool Start()
        {
            // Check if the process is running inside a snap 
            Console.WriteLine($"Running inside snap: {IsSnapped}");

            //Load the application data
            if (!Load())
            {
                return false;
            }

            //Start the HTTP Listener
            if (!StartListenHttp())
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// Stops the application
        /// </summary>
        /// <returns></returns>
        public bool Stop()
        {
            //Save the application data (discard result)
            if (!Save())
            {
                return false;
            }

            //Stop HTTP listening (discard result)
            StopListenHttp();

            return true;
        }

        #endregion

        #region Private

        /// <summary>
        /// Resets the application data
        /// </summary>
        /// <returns></returns>
        private bool Reset()
        {
            //Create new application data
            MyAppData = new AppData(
                HostName: Dns.GetHostName(),
                IsLinux: RuntimeInformation.IsOSPlatform(OSPlatform.Linux),
                OsArchitecture: RuntimeInformation.OSArchitecture.ToString(),
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
        private bool Load()
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
                return true;
            }
            catch (Exception exc) when (exc is IOException || exc is JsonException)
            {
                Console.WriteLine($"Loading application data from file '{path}' failed! {exc.Message}");
                return false;
            }
        }

        /// <summary>
        /// Saves (serializes) the application data into a file (JSON)
        /// </summary>
        /// <returns></returns>
        private bool Save()
        {
            //Ensure the storage location, first.
            if (!EnsureStorageLocation())
            {
                return false;
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
                return true;
            }
            catch (IOException exc)
            {
                Console.WriteLine($"Saving application data to file '{path}' failed! {exc.Message}");
                return false;
            }
        }

        /// <summary>
        /// Ensures the Storage Location
        /// </summary>
        /// <returns></returns>
        private static bool EnsureStorageLocation()
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
                    return false;
                }
            }

            return true;
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
        private static JsonWebToken GetToken(HttpListenerRequest request)
        {

            var authorization = request.Headers["Authorization"];
            if (authorization == null)
            {
                return null;
            }

            //Extract the token
            var token = authorization.Split(" ").Last();

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
                return null;
            }
#endif
            //Return the JWT
            return new JsonWebToken(token);
        }

        /// <summary>
        /// Starts the HTTP Listener
        /// </summary>
        /// <returns></returns>
        private bool StartListenHttp()
        {
            try
            {
                //Check if supported          
                if (!HttpListener.IsSupported)
                {
                    Console.WriteLine("HTTP Listening not supported!");
                    return false;
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
                    return false;
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
                        var jwt = GetToken(request);
                        if (jwt == null)
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

                            var appDataHttpRequest = JsonSerializer.Deserialize<AppDataHttpRequest>(reader.ReadToEnd(), new JsonSerializerOptions
                            {
                                PropertyNameCaseInsensitive = true
                            });

                            Console.WriteLine($"Payload: {appDataHttpRequest}");
                            if (request.Url == null)
                            {
                                return false;
                            }

                            var route = request.Url.ToString();

                            //Load Command
                            if (route.Equals(HttpApiRouteLoad))
                            {
                                //Phases
                                switch (appDataHttpRequest.Phase)
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
                                        response.StatusCode =
                                            Load()
                                                ? (int)HttpStatusCode.Accepted
                                                : (int)HttpStatusCode.InternalServerError;
                                        break;

                                    default:
                                        //We return 204 here to add upwards-compatibility for new phases
                                        response.StatusCode = (int)HttpStatusCode.NoContent;
                                        break;
                                }
                            }
                            else
                            {
                                //Save Command
                                if (route.Equals(HttpApiRouteSave))
                                {
                                    //Phases
                                    switch (appDataHttpRequest.Phase)
                                    {
                                        //save: Serialize current resources into active configuration
                                        case "save":
                                            //This is were we can save our application data into current configuration to be persistent
                                            response.StatusCode =
                                                Save()
                                                    ? (int)HttpStatusCode.Accepted
                                                    : (int)HttpStatusCode.InternalServerError;
                                            break;

                                        default:
                                            //We return 204 here to add upwards-compatibility for new phases
                                            response.StatusCode = (int)HttpStatusCode.NoContent;
                                            break;
                                    }
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
                return true;
            }
            catch (HttpListenerException exc)
            {
                Console.WriteLine($"Listening to HTTP failed! {exc.Message}");
                return false;
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
