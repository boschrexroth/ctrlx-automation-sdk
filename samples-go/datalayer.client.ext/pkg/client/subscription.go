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

import (
	"fmt"

	"github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"
)

type subscriptionClient struct {
	client *datalayer.Client
	sub    *datalayer.Subscription
}

func NewSubscriptionClient(c *datalayer.Client) *subscriptionClient {
	call := &subscriptionClient{client: c}
	return call
}

func DeleteSubscriptionClient(s *subscriptionClient) {
	if s == nil {
		return
	}
	if s.client == nil {
		return
	}
	if s.sub != nil {
		s.client.DeleteSubscription(s.sub)
	}
	s.client = nil
	s.sub = nil
}

/*
// global onSubscribe
func onSubscribe(result datalayer.Result, items map[string]datalayer.Variant) {
	if result != datalayer.ResultOk {
		fmt.Println("onSubscribe result: ", result)
		return
	}
	for k, v := range items {
		fmt.Println(k, " ", v.GetUint32())
	}
}
*/

// onSubscribe
func (s *subscriptionClient) onSubscribe(result datalayer.Result, items map[string]datalayer.Variant) {
	if result != datalayer.ResultOk {
		fmt.Println("onSubscribe result: ", result)
		return
	}
	for k, v := range items {
		fmt.Println(k, " ", v.GetUint32())
	}
}

func (s *subscriptionClient) Run() {

	//sub, r := s.client.CreateSubscription("mySub", datalayer.DefaultSubscriptionProperties(), onSubscribe)
	sub, r := s.client.CreateSubscription("mySub", datalayer.DefaultSubscriptionProperties(), s.onSubscribe)
	if r != datalayer.ResultOk {
		fmt.Println("CreateSubscription result: ", r)
		return
	}

	r = sub.Subscribe("framework/metrics/system/cpu-utilisation-percent",
		"framework/metrics/system/memused-percent")

	fmt.Println("Subscribe result: ", r)
}
