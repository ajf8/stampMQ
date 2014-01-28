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

#include "queue.hpp"

namespace stampmq {

Queue::Queue(const std::string& dstname) : Destination(dstname), queue_index_(0)
{
}

int Queue::size()
{
    return queue_.size();
}

// requires read lock
SubscriptionPtr Queue::GetNextSubscription()
{
    unsigned int subs = subscribers_.size();
    if (queue_index_ >= subs) {
        queue_index_ = 0;
    }
    return subscribers_[queue_index_++];
}

void Queue::Deliver(FramePtr frame)
{
    boost::upgrade_lock<boost::shared_mutex> uplock(subscribers_mtx_);

    if (subscribers_.size() > 0) {
        SubscriptionPtr sub = GetNextSubscription();
        sub->connection()->QueueMessage(sub, frame);
    } else {
        boost::unique_lock<boost::shared_mutex> wlock(queue_mtx_);
        queue_.push(frame);
    }
}

void Queue::ProcessQueue()
{
    boost::shared_lock<boost::shared_mutex> subslock(subscribers_mtx_);
    boost::upgrade_lock<boost::shared_mutex> quplock(queue_mtx_);
    if (!queue_.empty() && subscribers_.size() > 0) {
        boost::upgrade_to_unique_lock<boost::shared_mutex> qwlock(quplock);
        while (!queue_.empty() && subscribers_.size() > 0) {
            SubscriptionPtr sub = GetNextSubscription();
            sub->connection()->QueueMessage(sub, queue_.front());
            queue_.pop();
        }
    }
}

SubscriptionPtr Queue::Subscribe(ConnectionPtr conn, SUBSCRIPTION_TYPE type, const std::string &id)
{
    SubscriptionPtr sub = Destination::Subscribe(conn, type, id);
    ProcessQueue();
    return sub;
}

}
