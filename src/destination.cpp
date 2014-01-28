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

#include <iostream>
#include <memory>

#include "connection_exception.hpp"
#include "destination.hpp"

namespace stampmq {

Destination::Destination(const std::string& name) : name_(name), subscribers_()
{
}

SubscriptionPtr Destination::Subscribe(ConnectionPtr conn, SUBSCRIPTION_TYPE type, const std::string &id)
{
    boost::unique_lock<boost::shared_mutex> wlock(subscribers_mtx_);

    SubscriptionPtr sub = std::make_shared<Subscription>(shared_from_this(), conn, type, id);

    if (subscribers_.get<2>().count(sub) == 0)
        subscribers_.get<2>().insert(sub);
    else
        throw ConnectionException("subscription already exists.");

    return sub;
}

void Destination::Unsubscribe(SubscriptionPtr sub)
{
    boost::unique_lock<boost::shared_mutex> wlock(subscribers_mtx_);

    if (subscribers_.get<2>().count(sub) > 0)
        subscribers_.get<2>().erase(sub);
}

subscribers_container& Destination::subscribers()
{
    return subscribers_;
}

std::string Destination::name() {
    return name_;
}

}
