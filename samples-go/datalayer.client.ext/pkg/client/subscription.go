/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
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
