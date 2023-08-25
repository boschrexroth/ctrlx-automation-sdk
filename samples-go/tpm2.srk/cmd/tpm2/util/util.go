/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package util

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
)

func OpenTPMStreamSocket(path string) (io.ReadWriteCloser, error) {
	// If it's a regular file, then open it
	var rwc io.ReadWriteCloser
	fi, err := os.Stat(path)
	if err != nil {
		return nil, err
	}

	if fi.Mode()&os.ModeDevice != 0 {
		var f *os.File
		f, err = os.OpenFile(path, os.O_RDWR, 0600)
		if err != nil {
			return nil, err
		}
		rwc = io.ReadWriteCloser(f)
	} else if fi.Mode()&os.ModeSocket != 0 {
		rwc = NewEmulatorReadWriteCloser(path)
	} else {
		return nil, fmt.Errorf("unsupported TPM file mode %s", fi.Mode().String())
	}

	return rwc, nil
}

// dialer abstracts the net.Dial call so test code can provide its own net.Conn
// implementation.
type dialer func(network, path string) (net.Conn, error)

// EmulatorReadWriteCloser manages connections with a TPM over a Unix
// domain socket. EmulatorReadWriteCloser is not thread safe.
type EmulatorReadWriteCloser struct {
	path   string
	conn   net.Conn
	dialer dialer
}

// NewEmulatorReadWriteCloser stores information about a Unix domain socket to
// write to and read from.
func NewEmulatorReadWriteCloser(path string) *EmulatorReadWriteCloser {
	erw := EmulatorReadWriteCloser{
		path:   path,
		dialer: net.Dial,
	}
	var err error
	erw.conn, err = erw.dialer("unix", erw.path)
	if err != nil {
		return nil
	}
	return &erw
}

func (erw *EmulatorReadWriteCloser) Read(p []byte) (int, error) {
	n, err := erw.conn.Read(p)
	return n, err
}

func (erw *EmulatorReadWriteCloser) Write(p []byte) (int, error) {
	return erw.conn.Write(p)
}

func (erw *EmulatorReadWriteCloser) Close() error {
	if erw.conn == nil {
		return fmt.Errorf("Cannot call Close when no connection is open")
	}
	err := erw.conn.Close()
	erw.conn = nil
	return err
}

func StoreFile(reader io.Reader, filePath string) error {

	_, err := os.Stat(filePath)
	if !os.IsNotExist(err) {
		return err
	}

	fileHandle, err := os.OpenFile(filePath, os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		log.Printf("Cannot create file %s", filePath)
		return err
	}

	_, err = io.Copy(fileHandle, reader)

	fileHandle.Sync()
	fileHandle.Close()

	return err
}
