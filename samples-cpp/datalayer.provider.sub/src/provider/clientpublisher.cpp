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


ClientPublisher::ClientPublisher(comm::datalayer::ISubscription* subscription, std::recursive_mutex &mtx)
: m_subscription(*subscription)
, m_mtx(mtx)
{
  m_thread.reset(new std::thread([this] { publish(); }));
}

ClientPublisher::~ClientPublisher()
{
  std::cout << "INFO >> " << __func__ << std::endl;
  m_running = false;
  m_thread->join();
  std::cout << "INFO << " << __func__ << std::endl;
}

void ClientPublisher::publish() {
  int64_t counter = 0;
  const comm::datalayer::SubscriptionProperties * props = comm::datalayer::GetSubscriptionProperties(m_subscription.getProps()->getData());
  u_int32_t pub_interval = props->publishInterval();
  std::cout << "INFO >> " << __func__ << " : " << pub_interval << std::endl;

  while(m_running) {
    std::vector<comm::datalayer::NotifyItem> items;
    {
      //std::lock_guard<std::recursive_mutex> guard(m_mtx); -> deadlock with 'onUnsubscription'
      if (m_mtx.try_lock()) {
        auto nodes_list = m_subscription.getNodes();
        const char** nodes = nodes_list;
        for (size_t i = 0; i < nodes_list.getCount(); i++)
        {
          comm::datalayer::NotifyItem item;
          flatbuffers::FlatBufferBuilder builder;
          builder.Finish(comm::datalayer::CreateNotifyInfoDirect(builder, nodes[i], m_subscription.getTimestamp()));
          item.data.setValue(counter++);
          item.info.copyFlatbuffers(builder);
          items.push_back(item);
        }
        m_mtx.unlock();
      }
      else {
        // faster for 'onUnsubscription'
        continue;
      }
    }
    if (! items.empty() ){
      m_subscription.publish(comm::datalayer::DlResult::DL_OK, items);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(pub_interval));
  }
  std::cout << "INFO << " << __func__ << " : " << pub_interval << std::endl;
}
