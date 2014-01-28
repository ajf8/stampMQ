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

#include <log4cxx/logger.h>
#include <boost/thread.hpp>

#include "frame.hpp"
#include "destination.hpp"
#include "destination_factory.hpp"
#include "hub.hpp"

using namespace log4cxx;

namespace stampmq {

Hub::Hub()
{

}

bool Hub::DestinationExists(const std::string& dstname)
{
    return destinations_.count(dstname) > 0;
}

DestinationPtr Hub::GetDestination(const std::string& dstname)
{
    boost::upgrade_lock<boost::shared_mutex> uplock(destinations_mtx_);
    if (DestinationExists(dstname)) {
        return destinations_[dstname];
    } else {
        boost::upgrade_to_unique_lock<boost::shared_mutex> wlock(uplock);
        return destinations_[dstname] = DestinationFactory::create(dstname);
    }
}

/*void Hub::Unsubscribe(ConnectionPtr conn, const std::string& id)
{
    DestinationPtr dst = GetDestination(dstname);
    SubscriptionPtr sub = conn->
    dst->Unsubscribe(conn);
}*/

SubscriptionPtr Hub::Subscribe(ConnectionPtr conn, const std::string& dstname, SUBSCRIPTION_TYPE type, const std::string &id)
{
    DestinationPtr dst = GetDestination(dstname);
    return dst->Subscribe(conn, type, id);
}

void Hub::Send(const std::string& dstname, FramePtr frame)
{
    GetDestination(dstname)->Deliver(frame);
}

}
