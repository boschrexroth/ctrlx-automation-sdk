/**
 * MIT License
 *
 * Copyright (c) 2021-2023 Bosch Rexroth AG
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
