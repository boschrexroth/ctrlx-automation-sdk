/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package node

import (
	"fmt"
	"sync"
	"time"

	"github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/datalayer"
)

// ClientPublisher contains basic elements for managing client subscriptions
type ClientPublisher struct {
	Sub      *datalayer.ProviderSubscription
	SubMutex *sync.RWMutex
	stop     bool
	tick     *time.Ticker
	counter  int32
}

// NewClientPublisher creates a ClientPublisher instance
func NewClientPublisher(sub *datalayer.ProviderSubscription, mu *sync.RWMutex) *ClientPublisher {
	p := &ClientPublisher{
		Sub:      sub,
		SubMutex: mu,
		stop:     false,
		counter:  0,
	}
	return p
}

// Start starts simple publisching thread
func (p *ClientPublisher) Start() {
	fmt.Println("ClientPublisher: Start: ", p.Sub.GetUniqueId())
	p.stop = false
	go func() {
		interval := p.Sub.GetProps().PublishInterval()
		p.tick = time.NewTicker(time.Millisecond * time.Duration(interval))
		for range p.tick.C {
			if p.stop {
				return
			}
			p.publish()
		}
	}()
}

// Stop stops simple publisching thread
func (p *ClientPublisher) Stop() {
	fmt.Println("ClientPublisher: Stop: ", p.Sub.GetUniqueId())
	p.stop = true
	if p.tick != nil {
		p.tick.Stop()
	}
}

// publish publishing values to client subscription
func (p *ClientPublisher) publish() {
	l := []*datalayer.NotifyItemPublish{}
	p.SubMutex.RLock()
	for _, addr := range p.Sub.GetNodes() {
		n := datalayer.NewNotifyItemPublish(addr)
		p.counter++
		n.GetData().SetInt32(p.counter)
		l = append(l, n)
	}
	p.SubMutex.RUnlock()
	p.Sub.Publish(datalayer.ResultOk, l)
	datalayer.DeleteNotifyItemsPublish(l)
}
