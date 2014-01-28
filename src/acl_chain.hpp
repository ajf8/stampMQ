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

#ifndef STAMPMQ_ACL_CHAIN_HPP
#define STAMPMQ_ACL_CHAIN_HPP

#include <list>
#include <memory>
#include <string>

#include <boost/signals2.hpp>
#include <boost/utility.hpp>

#include "principal.hpp"
#include "acl.hpp"

namespace stampmq {

typedef boost::signals2::signal<void ()> invalidated_sig_t;

class AclChain : boost::noncopyable {
public:
    AclChain();
    void Add(Acl acl);
    bool IsAllowed(PrincipalPtr p, const std::string& destination, ACL_ACTION action);
    void Invalidate();
    invalidated_sig_t& invalidated();
private:
    invalidated_sig_t invalidated_sig_;
    boost::regex pattern_rx_;
    std::list<Acl> chain_;
};

typedef std::shared_ptr<AclChain> AclChainPtr;

}

#endif
