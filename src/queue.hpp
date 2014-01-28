/* Copyright 2013 ajf http://github.com/ajf8
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef STAMPMQ_QUEUE_HPP
#define STAMPMQ_QUEUE_HPP

#include <queue>

#include <boost/thread.hpp>

#include "connection.hpp"
#include "destination.hpp"
#include "send_frame.hpp"

namespace stampmq {

class Queue : public Destination {
public:
    Queue(const std::string& dstname);
    int size();
    void Deliver(FramePtr frame);
    void ProcessQueue();
    SubscriptionPtr Subscribe(ConnectionPtr conn, SUBSCRIPTION_TYPE type, const std::string &id);
protected:
    SubscriptionPtr GetNextSubscription();
    std::queue<FramePtr> queue_;
    boost::shared_mutex queue_mtx_;
    std::size_t queue_index_;
};

}

#endif
