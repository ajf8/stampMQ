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

#include <fstream>
#include <memory>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <log4cxx/logger.h>
#include <yaml-cpp/yaml.h>

#include "fatal_exception.hpp"
#include "acl.hpp"
#include "acl_chain.hpp"
#include "config.hpp"

namespace stampmq {

using namespace log4cxx;

LoggerPtr config_logger(Logger::getLogger("stampmq.Config"));

Config::Config()
{

}

void Config::Load(const std::string& file)
{
    if (!boost::filesystem::is_regular_file(boost::filesystem::path(file))) {
        throw FatalException((boost::format("config file %s does not exist.") % file).str());
    }
    yml_ = YAML::LoadFile(file);
}

YAML::Node Config::operator[] (const char *key)
{
    return yml_[key];
}

void Config::ImportAclAction(Acl& acl, const YAML::Node &acl_node, ACL_ACTION action, const char *key)
{
    const YAML::Node& action_node = acl_node[key];
    if (action_node) {
        std::list<std::string>& allowed = acl.GetAllowed(action);
        for (auto role_iter = action_node.begin(); role_iter != action_node.end(); ++role_iter) {
            std::string role = role_iter->as<std::string>("");
            LOG4CXX_TRACE (config_logger, "adding " + role + " to acl actions of type " + key);
            allowed.push_back(role);
        }
    }
}

AclChainPtr Config::MakeAcl()
{
    AclChainPtr aclc = std::make_shared<AclChain>();
    const YAML::Node& acl_node = (*this)["acl"];
    if (!acl_node) {
        LOG4CXX_WARN (config_logger, "no ACL chain in config. defaults will allow all users.");
        return aclc;
    }
    for (auto a = acl_node.begin(); a != acl_node.end(); a++) {
        const YAML::Node& destNode = (*a)["destination"];
        if (!destNode) {
            throw FatalException("acl missing destination.");
        } else {
            std::string dest = destNode.as<std::string>("");
            LOG4CXX_DEBUG (config_logger, "parsing acl for destination: "+dest);
            Acl acl(dest);
            ImportAclAction(acl, *a, read, "read");
            ImportAclAction(acl, *a, write, "write");
            ImportAclAction(acl, *a, admin, "admin");
            aclc->Add(acl);
        }
    }
    return aclc;
}

}
