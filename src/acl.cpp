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

#include "principal.hpp"
#include "acl.hpp"

namespace stampmq {

using namespace boost;

Acl::Acl(const std::string& pattern) : pattern_(pattern), pattern_rx_(pattern)
{

}

bool Acl::Matches(const std::string& destination)
{
    cmatch what;
    return regex_match(destination.c_str(), what, pattern_rx_);
}

std::list<std::string>& Acl::GetAllowed(ACL_ACTION action)
{
    switch (action) {
    case read:
        return readers_;
    case write:
        return writers_;
    case admin:
        return admins_;
    }
    throw std::exception();
}

void Acl::AddAllowed(const std::string& role, ACL_ACTION action)
{
    GetAllowed(action).push_back(role);
}

bool Acl::IsAllowed(PrincipalPtr p, ACL_ACTION action)
{
    std::list<std::string>& allowed = GetAllowed(action);
    for (std::list<std::string>::iterator acl_iter = allowed.begin(); acl_iter != allowed.end(); ++acl_iter) {
        if (p->HasRole(*acl_iter)) {
            return true;
        }
    }
    return false;
}

}
