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

#ifndef STAMPMQ_DESTINATION_HPP
#define STAMPMQ_DESTINATION_HPP

#include <boost/utility.hpp>
#include <boost/thread.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

#include <string>
#include <vector>
#include <memory>

//#include "frame.hpp"
#include "send_frame.hpp"
//#include "connection.hpp"
#include "subscription.hpp"

namespace stampmq {

class Connection;
typedef std::shared_ptr<Connection> ConnectionPtr;

class Frame;
typedef std::shared_ptr<Frame> FramePtr;

class Subscription;
typedef std::shared_ptr<Subscription> SubscriptionPtr;

using namespace boost::multi_index;

typedef boost::multi_index_container<
SubscriptionPtr,
indexed_by<
random_access<>,
sequenced<>,
ordered_unique<identity<SubscriptionPtr>>
>
> subscribers_container;

class Destination : public std::enable_shared_from_this<Destination>, boost::noncopyable {
public:
    Destination(const std::string& name);
    virtual void Deliver(FramePtr frame) = 0;
    virtual SubscriptionPtr Subscribe(ConnectionPtr conn, SUBSCRIPTION_TYPE type, const std::string &id);
    virtual void Unsubscribe(SubscriptionPtr sub);
    subscribers_container& subscribers();
    std::string name();
protected:
    std::string name_;
    subscribers_container subscribers_;
    boost::shared_mutex subscribers_mtx_;
};

typedef std::shared_ptr<Destination> DestinationPtr;

}

#endif // end STAMPMQ_DESTINATION_HPP
