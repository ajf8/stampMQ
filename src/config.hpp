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

#ifndef STAMPMQ_CONFIG_HPP
#define STAMPMQ_CONFIG_HPP

#include <memory>

#include <yaml-cpp/yaml.h>
#include <boost/utility.hpp>

#include "acl_chain.hpp"

namespace stampmq {

class Config : public std::enable_shared_from_this<Config>, boost::noncopyable {
public:
    Config();
    void Load(const std::string& file);
    YAML::Node operator[](const char *key);
    AclChainPtr MakeAcl();
protected:
    void ImportAclAction(Acl& acl, const YAML::Node &acl_node, ACL_ACTION action, const char *key);
    YAML::Node yml_;
};

typedef std::shared_ptr<Config> ConfigPtr;

}

#endif
