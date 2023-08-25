/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package client

import "github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"

type bulkClient struct {
	client *datalayer.Client
}

func NewBulkClient(c *datalayer.Client) *bulkClient {
	call := &bulkClient{client: c}
	return call
}

func DeleteBulkClient(s *bulkClient) {
	if s == nil {
		return
	}
	if s.client == nil {
		return
	}
	s.client = nil
}

// runs bulk example once
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
		println("Addresse: ", value)
		println("Result  : ", bulk.Response[key].Result)
		data := bulk.Response[key].Data
		println("Value   : ", data.GetFloat64())
		println("Time    :", bulk.Response[key].Time.String())
	}
}
