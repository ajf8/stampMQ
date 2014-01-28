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

#include <string>

#include <log4cxx/logger.h>
#include <boost/signals2.hpp>

#include "acl.hpp"
#include "acl_chain.hpp"

namespace stampmq {

using namespace log4cxx;

LoggerPtr acl_chain_logger(Logger::getLogger("stampmq.AclChain"));

AclChain::AclChain() : chain_()
{

}

void AclChain::Add(Acl acl)
{
    chain_.push_back(acl);
}

bool AclChain::IsAllowed(PrincipalPtr p, const std::string& destination, ACL_ACTION action)
{
    bool allowed = true;
    for (auto iter = chain_.begin(); iter != chain_.end(); iter++) {
        if (iter->Matches(destination)) {
            allowed = iter->IsAllowed(p, action);
        }
    }
    return allowed;
}

void AclChain::Invalidate()
{
    chain_.clear();
    invalidated_sig_();
}

invalidated_sig_t& AclChain::invalidated()
{
    return invalidated_sig_;
}

}
