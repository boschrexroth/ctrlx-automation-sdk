/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"log"
	"net"
	"net/http"
	"os"
	"path/filepath"
	"runtime"
	"strconv"
	"strings"
	"time"

	"github.com/golang-jwt/jwt"
)

const (
	//Setup HTTP listener and load/save routes we're interested in
	RestUrlLoad = "sdk-go-appdata/api/v1/load"
	RestUrlSave = "sdk-go-appdata/api/v1/save"

	// The name of the storage file
	StorageFileName = "appdata.json"

	// The name of the application storage folder
	// MUST be same as specified in your *.package-manifest.json file
	StorageFolderName = "sdk-go-appdata"
)

var (
	//We have to choose a free port we're listening on
	HttpPort = 5555
	// Gets the base storage location for all applications
	BaseStorageLocation = filepath.Join(os.Getenv("SNAP_COMMON"), "solutions", "activeConfiguration")
	// Gets the application data storage location
	StorageLocation = filepath.Join(BaseStorageLocation, StorageFolderName)
	// Gets the application data storage file
	StorageFile = filepath.Join(StorageLocation, StorageFileName)
	// Represents our application data to be loaded/saved as JSON
	MyAppData AppData
)

// Represents our application data to be loaded/saved as JSON
type AppData struct {
	HostName       string
	OS             string
	OSArchitecture string
	TimeStamp      time.Time
	SecretNumber   int
}

// Represents an application data HTTP request command
type AppDataHttpRequest struct {
	ConfigurationPath string
	Id                string
	Phase             string
}

// Indicates whether the application is running inside a snap environment.
func isSnap() bool {
	return os.Getenv("SNAP") != ""
}

var listener net.Listener

// Starts the AppData listener.
func StartAppDataServer() error {

	// Depending on running as snap or as external app, start webserver with unix sockets or tcp
	if isSnap() {
		return unixSocketServe()
	} else {
		// Create http handle
		http.HandleFunc("/"+RestUrlLoad, handler)
		http.HandleFunc("/"+RestUrlSave, handler)

		fmt.Print("app is not running as snap\n")
		var err error
		listener, err = net.Listen("tcp", ":"+strconv.Itoa(HttpPort))
		if err != nil {
			return err
		}
		fmt.Printf("server UP - TCP - Port: %s \n", strconv.Itoa(HttpPort))
		http.Serve(listener, nil)
		fmt.Printf("server DOWN - TCP - Port: %s \n", strconv.Itoa(HttpPort))

	}
	return nil
}

// StopAppDataListener stopps the AppData listener
func StopAppDataListener() {
	if listener == nil {
		return
	}

	listener.Close()
	fmt.Println("stopped listening")
}

func unixSocketServe() error {
	fmt.Println("app is running as snap")
	sockpath := filepath.Join(os.Getenv("SNAP_DATA"), "/package-run/sdk-go-appdata/")
	sockfile := filepath.Join(sockpath, "web.sock")
	//remo
	if _, err := os.Stat(sockpath); err == nil {
		fmt.Println("unix domain socket already exists, deleting")
		_ = os.Remove(sockfile)
	}

	if _, err := os.Stat(sockpath); os.IsNotExist(err) {
		fmt.Println("unix domain socket creating")
		os.MkdirAll(sockpath, os.ModePerm)
	}
	fmt.Println("app socket listening")
	var err error
	listener, err = net.Listen("unix", sockfile)
	if err != nil {
		fmt.Println("app Socket error: ", err)
		return err
	}

	fmt.Print("app socket register handler\n")
	m := http.NewServeMux()
	m.HandleFunc("/"+RestUrlLoad, handler)
	m.HandleFunc("/"+RestUrlSave, handler)

	server := http.Server{
		Handler: m,
	}

	fmt.Printf("server UP - UNIX SOCKET - File: %s ", sockfile)
	if err := server.Serve(listener); err != nil {
		log.Fatal(err)
		return err
	}
	fmt.Printf("server DOWN - UNIX SOCKET - File: %s ", sockfile)
	return nil
}

func handler(response http.ResponseWriter, request *http.Request) {
	// Check if POST method is used
	if request.Method != http.MethodPost {
		fmt.Printf("request method %s not allowed \n", request.Method)
		response.WriteHeader(http.StatusMethodNotAllowed)
		return
	}

	//We received a new app data REST command
	//fmt.Fprint(response, "REST Command: ", request.URL.Path[1:])
	restUrl := request.URL.Path[1:]
	fmt.Printf("REST Command: %s \n ", restUrl)

	// Get token from http request
	token, err := getToken(request)

	if err != nil {
		fmt.Print("invalid authentication token (Bearer)")
		response.WriteHeader(http.StatusUnauthorized)
		return
	}

	if !isAuthorized(token) {
		fmt.Print("not authorized (Bearer invalid)")
		response.WriteHeader(http.StatusUnauthorized)
		return
	}

	appdataRequest := AppDataHttpRequest{}
	decoder := json.NewDecoder(request.Body)

	err = decoder.Decode(&appdataRequest)
	if err != nil {
		fmt.Printf("error decoding json payload: %s", err.Error())
		response.WriteHeader(http.StatusBadRequest)
		return
	}

	// Load Command
	if restUrl == RestUrlLoad {
		// Phases
		switch appdataRequest.Phase {
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
			response.WriteHeader(http.StatusNoContent)
		// load: Provide resources according to the data from the active configuration
		case "load":
			//This is were we can load our application data from current configuration
			err = loadAppData()
			if err != nil {
				response.WriteHeader(http.StatusInternalServerError)
			}
			response.WriteHeader(http.StatusAccepted)

		default:
			//We return 204 here to add upwards-compatibility for new phases
			response.WriteHeader(http.StatusNoContent)
		}

		// Save Command
	} else if restUrl == RestUrlSave {
		// Phases
		switch appdataRequest.Phase {
		//save: Serialize current resources into active configuration
		case "save":
			//This is were we can save our application data into current configuration to be persistent
			err = saveAppData()
			if err != nil {
				response.WriteHeader(http.StatusInternalServerError)
			}
			response.WriteHeader(http.StatusAccepted)

		default:
			//We return 204 here to add upwards-compatibility for new phases
			response.WriteHeader(http.StatusNoContent)
		}
	}
}

func loadAppData() error {
	path := StorageFile

	if _, err := os.Stat(path); os.IsNotExist(err) {
		return resetAppData()
	}

	jsonFile, err := os.ReadFile(path)
	if err != nil {
		fmt.Printf("Loading application data from file '%s' failed! %s", path, err.Error())
		return err
	}

	data := AppData{}
	err = json.Unmarshal([]byte(jsonFile), &data)
	if err != nil {
		fmt.Printf("Loading application data from file '%s' failed! %s", path, err.Error())
		return err
	}
	fmt.Printf("Loaded application data from file '%s'.", path)
	return nil
}

func resetAppData() error {
	MyAppData.HostName, _ = os.Hostname()
	MyAppData.OS = runtime.GOOS
	MyAppData.OSArchitecture = runtime.GOARCH
	MyAppData.TimeStamp = time.Now()
	MyAppData.SecretNumber = 4711

	fmt.Print("Created new application data.\n")

	return saveAppData()
}

func saveAppData() error {
	err := ensureStorageLocation()
	if err != nil {
		return err
	}
	path := StorageFile

	jsonb, err := json.Marshal(MyAppData)
	if err != nil {
		fmt.Printf("Saving application data to file '%s' failed! %s\n", path, err.Error())
		return err
	}

	err = os.WriteFile(path, jsonb, 0644)
	if err != nil {
		fmt.Printf("Saving application data to file '%s' failed! %s\n", path, err.Error())
		return err
	}
	fmt.Printf("Saved application data to file: '%s'  \n", path)
	return nil
}

func ensureStorageLocation() error {
	path := StorageLocation

	if _, err := os.Stat(path); os.IsNotExist(err) {
		err := os.MkdirAll(path, os.ModePerm)
		if err != nil {
			fmt.Printf("Creating storage location '%s' failed! %s\n", path, err.Error())
			return err

		}
		fmt.Printf("Created storage location: '%s'\n", path)

	}
	return nil
}

func getToken(request *http.Request) (*jwt.Token, error) {
	//Extract the token
	authHeader := request.Header.Get("Authorization")
	if authHeader != "" {
		bearerToken := strings.Split(authHeader, " ")
		if len(bearerToken) == 2 {
			token, _ := jwt.Parse(bearerToken[1], func(token *jwt.Token) (interface{}, error) {
				return []byte("AllYourBase"), nil
			})
			/* SIGNATURE CHECKING DISABLED
			fmt.Printf("Token Valid: %t\n", token.Valid)
			if err != nil {
				fmt.Printf("Error parsing jsob web token: '%s'\n", err.Error())
			}
			//return token, err
			*/
			return token, nil
		}
		return nil, errors.New("invalid authorization token")
	}
	return nil, errors.New("an authorization header is required")
}

// Indicates whether the request is authorized to be executed
func isAuthorized(token *jwt.Token) bool {
	if claims, ok := token.Claims.(jwt.MapClaims); ok {
		checkScopes := claims["scope"].([]interface{})

		for _, role := range checkScopes {
			if role == "rexroth-device.all.rwx" {
				fmt.Println("Check scope authorization passed")
				return true
			}
		}

	}
	fmt.Println("Check scope authorization failed")
	return false
}
