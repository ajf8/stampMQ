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

#ifndef STAMPMQ_ACL_HPP
#define STAMPMQ_ACL_HPP

#include <boost/regex.hpp>

#include "principal.hpp"

namespace stampmq {

enum ACL_ACTION {
    read,
    write,
    admin
};

class Acl {
public:
    Acl(const std::string& pattern);
    bool Matches(const std::string& destination);
    std::list<std::string>& GetAllowed(ACL_ACTION action);
    bool IsAllowed(PrincipalPtr p, ACL_ACTION action);
    void AddAllowed(const std::string& role, ACL_ACTION action);
private:
    std::string pattern_;
    boost::regex pattern_rx_;
    std::list<std::string> readers_;
    std::list<std::string> writers_;
    std::list<std::string> admins_;
};

}

#endif
