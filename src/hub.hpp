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

#ifndef STAMPMQ_HUB_HPP
#define STAMPMQ_HUB_HPP

#include <boost/utility.hpp>
#include <boost/thread.hpp>

//#include "frame.hpp"
//#include "destination.hpp"
#include "subscription.hpp"

namespace stampmq {

class Frame;
typedef std::shared_ptr<Frame> FramePtr;
class Destination;
typedef std::shared_ptr<Destination> DestinationPtr;
class Subscription;
typedef std::shared_ptr<Subscription> SubscriptionPtr;

class Hub : boost::noncopyable {
public:
    Hub();
    bool DestinationExists(const std::string& dstname);
    DestinationPtr GetDestination(const std::string& dstname);
    //void Unsubscribe(ConnectionPtr conn, const std::string& dstname);
    SubscriptionPtr Subscribe(ConnectionPtr conn, const std::string& dstname, SUBSCRIPTION_TYPE type, const std::string &id);
    void Send(const std::string& dstname, FramePtr frame);
private:
    std::map<std::string, DestinationPtr> destinations_;
    boost::shared_mutex destinations_mtx_;
};

typedef std::shared_ptr<Hub> HubPtr;

}

#endif
