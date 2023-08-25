/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package client

import (
	"github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"
)

type CallDatalayerClient struct {
	client *datalayer.Client
	sub    *subscriptionClient
}

func NewCallDatalayerClient(c *datalayer.Client) *CallDatalayerClient {
	call := &CallDatalayerClient{client: c}
	return call
}

func DeleteCallDatalayerClient(c *CallDatalayerClient) {
	if c == nil {
		return
	}
	if c.client == nil {
		return
	}
	DeleteSubscriptionClient(c.sub)
	datalayer.DeleteClient(c.client)
	c.sub = nil
	c.client = nil
}

func (c *CallDatalayerClient) Run() {

	c.runBulk()

	c.runSubscription()
}

func (c *CallDatalayerClient) runBulk() {
	b := NewBulkClient(c.client)
	defer DeleteBulkClient(b)
	b.Run()
}

func (c *CallDatalayerClient) runSubscription() {
	c.sub = NewSubscriptionClient(c.client)
	c.sub.Run()
}
