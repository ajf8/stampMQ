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

#include <exception>
#include <string>
#include <list>

#include <boost/regex.hpp>

#include "policy.hpp"

namespace stampmq {

using namespace boost;

Policy::Policy(const std::string& pattern) : pattern_(pattern), pattern_rx_(pattern), queue_depth_(-1)
{

}

bool Policy::Matches(const std::string& destination)
{
    cmatch what;
    return regex_match(destination.c_str(), what, pattern_rx_);
}

long Policy::queue_depth()
{
    return queue_depth_;
}

void Policy::set_queue_depth(long qd)
{
    queue_depth_ = qd;
}

}
