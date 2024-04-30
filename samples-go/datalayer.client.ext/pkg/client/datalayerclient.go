/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package client

import (
	"github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/datalayer"
)

// datalayerClient is the a client wrapper.
type datalayerClient struct {
	client *datalayer.Client
	sub    *subscriptionClient
}

// NewDatalayerClient creates a client wrapper.
func NewDatalayerClient(c *datalayer.Client) *datalayerClient {
	client := &datalayerClient{client: c}
	return client
}

// Delete deletes the client wrapper.
func (c *datalayerClient) Delete() {
	if c.client == nil {
		return
	}
	if c.sub != nil {
		c.sub.Delete()
	}
	datalayer.DeleteClient(c.client)
	c.sub = nil
	c.client = nil
}

// Run runs the operations.
func (c *datalayerClient) Run() {
	c.runBulk()
	c.runSubscription()
}

// runBulk runs the Bulk operation.
func (c *datalayerClient) runBulk() {
	b := NewBulkClient(c.client)
	b.Run()
}

// runSubscription stars the subscription.
func (c *datalayerClient) runSubscription() {
	c.sub = NewSubscriptionClient(c.client)
	c.sub.Run()
}
