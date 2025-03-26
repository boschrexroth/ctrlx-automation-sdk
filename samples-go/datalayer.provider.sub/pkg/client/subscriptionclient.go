/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package client

import (
	"fmt"

	"github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/datalayer"
)

// subscriptionClient is a subscription client wrapper.
type subscriptionClient struct {
	client *datalayer.Client
	sub    *datalayer.Subscription
}

// NewSubscriptionClient creates a subscription client wrapper.
func NewSubscriptionClient(c *datalayer.Client) *subscriptionClient {
	client := &subscriptionClient{client: c}
	return client
}

// Delete deletes a subscription client wrapper.
func (s *subscriptionClient) Delete() {
	if s.client == nil {
		return
	}
	if s.sub != nil {
		s.client.DeleteSubscription(s.sub)
	}
	datalayer.DeleteClient(s.client)
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

// onSubscribe is the subscription callback handler.
func (s *subscriptionClient) onSubscribe(result datalayer.Result, items map[string]datalayer.Variant) {
	if result != datalayer.ResultOk {
		fmt.Println("onSubscribe result: ", result)
		return
	}
	for k, v := range items {
		fmt.Println(k, " ", v.GetInt32())
	}
}

// Run starts the subscription.
func (s *subscriptionClient) Run() {

	//sub, r := s.client.CreateSubscription("mySub", datalayer.DefaultSubscriptionProperties(), onSubscribe)
	sub, r := s.client.CreateSubscription("myGoSub", datalayer.DefaultSubscriptionProperties(), s.onSubscribe)
	if r != datalayer.ResultOk {
		fmt.Println("CreateSubscription result: ", r)
		return
	}

	r = sub.Subscribe("sdk/go/provider/sub/int1")
	fmt.Println("Subscribe result: ", r)

	r = sub.Subscribe("sdk/go/provider/sub/int2")
	fmt.Println("Subscribe result: ", r)
}
