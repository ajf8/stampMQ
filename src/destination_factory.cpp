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

#include <memory>
#include <exception>
#include <boost/algorithm/string/predicate.hpp>

#include "destination.hpp"
#include "topic.hpp"
#include "queue.hpp"
#include "connection_exception.hpp"
#include "destination_factory.hpp"

namespace stampmq {

DestinationPtr DestinationFactory::create(std::string dstname)
{
    //name = boost::to_upper(name);
    if (boost::starts_with(dstname, "/topic/")) {
        return std::make_shared<Topic>(dstname);
    } else if (boost::starts_with(dstname, "/queue/")) {
        return std::make_shared<Queue>(dstname);
    } else {
        throw ConnectionException("destinations must begin with /queue/ or /topic/");
    }
}

}
