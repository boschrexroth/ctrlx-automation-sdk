/*
* SPDX-FileCopyrightText: Bosch Rexroth AG
*
* SPDX-License-Identifier: MIT
*/

#ifndef CLIENT_PUBLISHER_H
#define CLIENT_PUBLISHER_H

#include <thread>
#include <mutex>

#include <comm/datalayer/datalayer.h>

// ClientPublisher manages a dedicated publish thread for a single ctrlX Data Layer subscription.
// It periodically reads all subscribed nodes and pushes notify items to the subscriber.
class ClientPublisher final
{
  // Reference to the subscription this publisher serves
  comm::datalayer::ISubscription &m_subscription;

  // Background thread that runs the publish loop
  std::unique_ptr<std::thread>    m_thread;

  // Flag to signal the publish loop to stop
  bool                            m_running = true;

  // Shared mutex to synchronize access with the provider node (e.g. onUnsubscription)
  std::recursive_mutex            &m_mtx;

  // Main publish loop: collects notify items for all subscribed nodes and calls publish()
  void publish();

public:
  // Constructs the publisher and starts the background publish thread.
  // @param subscription  Pointer to the active Data Layer subscription (must outlive this object)
  // @param mtx           Shared recursive mutex used to guard subscription map access
  ClientPublisher(comm::datalayer::ISubscription* subscription, std::recursive_mutex &mtx);

  // Stops the publish loop and joins the background thread
  ~ClientPublisher();
};

#endif //CLIENT_PUBLISHER_H
