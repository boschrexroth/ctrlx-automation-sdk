/*
* SPDX-FileCopyrightText: Bosch Rexroth AG
*
* SPDX-License-Identifier: MIT
*/

#include <iostream>
#include <vector>

#include <comm/datalayer/sub_properties_generated.h>
#include <comm/datalayer/notify_info_generated.h>

#include "clientpublisher.h"


// Constructor: stores the subscription reference and shared mutex, then starts the background publish thread
ClientPublisher::ClientPublisher(comm::datalayer::ISubscription* subscription, std::recursive_mutex &mtx)
: m_subscription(*subscription)
, m_mtx(mtx)
{
  // Launch the publish loop in a separate thread
  m_thread.reset(new std::thread([this] { publish(); }));
}

// Destructor: signals the publish loop to stop and waits for the thread to finish
ClientPublisher::~ClientPublisher()
{
  std::cout << "INFO >> " << __func__ << std::endl;
  m_running = false;   // Signal the publish loop to exit
  m_thread->join();    // Wait for the thread to finish cleanly
  std::cout << "INFO << " << __func__ << std::endl;
}

// Publish loop: runs in a background thread and pushes notify items to the subscriber
// at the interval defined by the subscription properties.
void ClientPublisher::publish() {
  int64_t counter = 0;

  // Read the publish interval from the subscription properties (in milliseconds)
  const comm::datalayer::SubscriptionProperties * props = comm::datalayer::GetSubscriptionProperties(m_subscription.getProps()->getData());
  u_int32_t pub_interval = props->publishInterval();
  std::cout << "INFO >> " << __func__ << " : " << pub_interval << std::endl;

  while(m_running) {
    std::vector<comm::datalayer::NotifyItem> items;
    {
      // try_lock is used instead of lock_guard to avoid a deadlock with onUnsubscription,
      // which holds the same mutex while destroying this object.
      if (m_mtx.try_lock()) {
        auto nodes_list = m_subscription.getNodes();
        const char** nodes = nodes_list;

        // Build one NotifyItem per subscribed node
        for (size_t i = 0; i < nodes_list.getCount(); i++)
        {
          comm::datalayer::NotifyItem item;
          flatbuffers::FlatBufferBuilder builder;
          // Fill notify info with the node address and the current subscription timestamp
          builder.Finish(comm::datalayer::CreateNotifyInfoDirect(builder, nodes[i], m_subscription.getTimestamp()));
          item.data.setValue(counter++);  // Increment counter as the published data value
          item.info.copyFlatbuffers(builder);
          items.push_back(item);
        }
        m_mtx.unlock();
      }
      else {
        // Lock not acquired — skip this cycle to allow onUnsubscription to proceed quickly
        continue;
      }
    }

    // Publish all collected items to the subscriber if the list is not empty
    if (! items.empty() ){
      m_subscription.publish(comm::datalayer::DlResult::DL_OK, items);
    }

    // Wait for the next publish interval before sending the next batch
    std::this_thread::sleep_for(std::chrono::milliseconds(pub_interval));
  }
  std::cout << "INFO << " << __func__ << " : " << pub_interval << std::endl;
}
