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

class ClientPublisher final
{
comm::datalayer::ISubscription &m_subscription;
std::unique_ptr<std::thread>    m_thread;

bool                            m_running = true;
std::recursive_mutex            &m_mtx;

void publish();

public:
  ClientPublisher(comm::datalayer::ISubscription* subscription, std::recursive_mutex &mtx);
  ~ClientPublisher();
};

#endif //CLIENT_PUBLISHER_H