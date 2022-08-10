/**
 * MIT License
 *
 * Copyright (c) 2021-2022 Bosch Rexroth AG
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package utils

import (
	"os"
	"strconv"
)

/**
* This type provides auxiliary methods to create ctrlX Datalayer client and provider connection strings to ctrlX CORE devices.
* It can be used for both running in an app build environment(QEMU VM) and within the snap environment on the ctrlX CORE.
*
* Feel free to use it in your projects and to change it if necessary.
*
* For ease of use, the default values for IP address, user, password and SSL port are chosen to match the settings of a
* newly created ctrlX CORE device:
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
*  1. ctrlX CORE or ctrlX CORE virtual with another IP address, user and password:
*
*     var connection = utils.NewConnectionBuilder().Ip("192.168.1.100").User("admin").Password("-$_U/{X$aG}Z3/e<"").Build()
*
*  2. ctrlX CORE virtual with port forwarding running on the same host as the app build environment (QEMU VM):
*
*     var connection = utils.NewConnectionBuilder().Ip("10.0.2.2").SslPort(443).Build()
*
*     Remarks:
*     10.0.2.2 is the IP address of the host from the point of view of the app build environment(QEMU VM).
*     8443 is the host port which is forwarded to the SSL port(=433) of the ctrlX CORE virtual
*
* IMPORTANT:
* You need not change the parameter settings before building a snap and installing the snap on a ctrlX CORE.
* The method detects the snap environment and uses automatically inter process communication.
* Therefor the connection string to the ctrlX Datalayer is: "ipc://"
 */

type ConnectionBuilder struct {
	ip       string
	user     string
	password string
	sslPort  int
}

// Creates a new ConnectionBuilder with default values
func NewConnectionBuilder() *ConnectionBuilder {
	cb := &ConnectionBuilder{}
	cb.ip = "192.168.1.1"
	cb.user = "boschrexroth"
	cb.password = "boschrexroth"
	cb.sslPort = 443
	return cb
}

// IP address of the ctrlX CORE. Use "10.0.2.2" to connect to a ctrlX CORE virtual with port forwarding
func (cb *ConnectionBuilder) Ip(ip string) *ConnectionBuilder {
	cb.ip = ip
	return cb
}

// Port number for a SSL connection. ctrlX CORE virtual with port forwarding: Use the host port (default 8443) forwarded to port 22 of the ctrlX virtual
func (cb *ConnectionBuilder) SslPort(sslPort int) *ConnectionBuilder {
	cb.sslPort = sslPort
	return cb
}

// Name of the user
func (cb *ConnectionBuilder) User(user string) *ConnectionBuilder {
	cb.user = user
	return cb
}

// Password of the user
func (cb *ConnectionBuilder) Password(password string) *ConnectionBuilder {
	cb.password = password
	return cb
}

// Get the connection string: ipc connection string if running inside snap otherwise tcp connection string
func (cb *ConnectionBuilder) Build() string {
	if os.Getenv("SNAP") != "" {
		return "ipc://"
	}

	return "tcp://" + cb.user + ":" + cb.password + "@" + cb.ip + "?sslport=" + strconv.Itoa(cb.sslPort)
}
