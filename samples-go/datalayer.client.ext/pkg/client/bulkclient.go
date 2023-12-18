/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package client

import "github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"

// bulkClient is the a bulk client wrapper.
type bulkClient struct {
	client *datalayer.Client
}

// NewBulkClient creates a new bulk client wrapper.
func NewBulkClient(c *datalayer.Client) *bulkClient {
	client := &bulkClient{client: c}
	return client
}

// Run runs bulk operation once.
func (b *bulkClient) Run() {

	bulk := b.client.CreateBulk()
	defer datalayer.DeleteBulk(bulk)
	m := []string{
		"framework/metrics/system/cpu-utilisation-percent",
		"framework/metrics/system/memavailable-mb",
	}

	args := []datalayer.BulkReadArg{}
	for _, value := range m {
		arg := datalayer.BulkReadArg{Address: value, Argument: nil}
		args = append(args, arg)
	}

	r := bulk.Read(args...)
	if r != datalayer.ResultOk {
		return
	}

	for key, value := range m {
		println("address: ", value)
		println("result  : ", bulk.Response[key].Result)
		data := bulk.Response[key].Data
		println("value   : ", data.GetFloat64())
		println("time    :", bulk.Response[key].Time.String())
	}
}
