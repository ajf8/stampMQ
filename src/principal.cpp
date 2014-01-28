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
#include <sstream>
#include <string>
#include <set>

#include "principal.hpp"

namespace stampmq {

Principal::Principal() : roles_(), authenticated_(false)
{
}

const std::string& Principal::name()
{
    return name_;
}

void Principal::set_name(const std::string& name)
{
    name_ = name;
}

void Principal::AddRole(const std::string& role)
{
    roles_.insert(role);
}

bool Principal::HasRole(const std::string& role)
{
    return roles_.count(role) > 0;
}

const std::set<std::string>& Principal::roles()
{
    return roles_;
}

std::string Principal::RolesAsString()
{
    std::stringstream ss;
    for(auto iter = roles_.begin(); iter != roles_.end(); ++iter) {
        if(iter != roles_.begin())
            ss << ",";
        ss << *iter;
    }
    return ss.str();
}

bool Principal::IsAuthenticated()
{
    return authenticated_;
}

void Principal::set_authenticated(bool authenticated)
{
    authenticated_ = authenticated;
}

} // namespace stampmq
