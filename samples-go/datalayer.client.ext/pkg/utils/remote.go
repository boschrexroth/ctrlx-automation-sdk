/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

package utils

import (
	"fmt"
	"os"
)

/**
* This type provides auxiliary methods to create ctrlX Datalayer client and provider connection strings to ctrlX OS devices.
* It can be used for both running in an app build environment (QEMU VM) and within the snap environment on the ctrlX OS.
*
* Feel free to use it in your projects and to change it if necessary.
*
* For ease of use, the default values for IP address, user, password and SSL port are chosen to match the settings of a
* newly created ctrlX OS device:
*
*     ip="192.168.1.1"
*     user="boschrexroth"
*     password="boschrexroth"
*     sslPort=443
*
* with these variables, the tcp connection string can be formatted as follows:
*
*     tcp://{user}:{password}@{ip}?sslport={sslPort}
*
* If these values do not suit your use case, explicitly pass the parameters that require different values.
*
* Here some examples:
*
*  1. ctrlX OS or ctrlX COREvirtual with another IP address, user and password:
*
*     var remote = utils.NewRemote().Ip("192.168.1.100").User("admin").Password("-$_U/{X$aG}Z3/e<"").Build()
*
*  2. ctrlX COREvirtual with port forwarding running on the same host as the app build environment (QEMU VM):
*
*     var remote = utils.NewRemote().Ip("10.0.2.2").SslPort(8443).Build()
*
*     Remarks:
*     10.0.2.2 is the IP address of the host from the point of view of the app build environment(QEMU VM).
*     8443 is the host port which is forwarded to the SSL port(=443) of the ctrlX COREvirtual
*
* IMPORTANT:
* You need not change the parameter settings before building a snap and installing the snap on a ctrlX OS.
* The Remote util detects the snap environment and uses automatically inter process communication.
* Therefore the connection string to the ctrlX Datalayer is: "ipc://"
*
* If you don't wan't to communicate to localhost (e.g. C2C), please build up the remote string manually in the following form (sslport is optional):
*
*     var remote := "tcp://{User}:{Password}@{Ip}?sslport={SslPort}";
*
* If you have a customized TCP port, different from 2069 for clients or 2070 for providers (e.g. for port forwarding):
*
*     var remote = utils.NewRemote().Ip("10.0.2.2").Port(4711).Build()
 */

type Remote struct {
	ip       string
	user     string
	password string
	sslPort  int
	port     int
}

// Creates a new Remote with default values
func NewRemote() *Remote {
	cb := &Remote{}
	cb.ip = "192.168.1.1"
	cb.user = "boschrexroth"
	cb.password = "boschrexroth"
	cb.sslPort = 443
	cb.port = 0
	return cb
}

// Ip sets the address of the ctrlX OS device.
func (cb *Remote) Ip(ip string) *Remote {
	cb.ip = ip
	return cb
}

// Port sets the port for the SSL connection.
func (cb *Remote) SslPort(sslPort int) *Remote {
	cb.sslPort = sslPort
	return cb
}

// Port sets the port for the TCP connection.
func (cb *Remote) Port(port int) *Remote {
	cb.port = port
	return cb
}

// Name sets the name of the user.
func (cb *Remote) User(user string) *Remote {
	cb.user = user
	return cb
}

// Password sets the password of the user.
func (cb *Remote) Password(password string) *Remote {
	cb.password = password
	return cb
}

// Builds the connection string: 'ipc' if running inside a snap, otherwise 'tcp'.
func (cb *Remote) Build() string {
	if os.Getenv("SNAP") != "" {
		return "ipc://"
	}

	if cb.port > 0 {
		return fmt.Sprintf("tcp://%s:%s@%s:%d?sslport=%d", cb.user, cb.password, cb.ip, cb.port, cb.sslPort)
	}
	return fmt.Sprintf("tcp://%s:%s@%s?sslport=%d", cb.user, cb.password, cb.ip, cb.sslPort)
}
