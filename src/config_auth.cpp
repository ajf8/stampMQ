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
#include <memory>

#include <yaml-cpp/yaml.h>

#include "config.hpp"
#include "authenticator.hpp"
#include "principal.hpp"
#include "connection_exception.hpp"
#include "fatal_exception.hpp"
#include "config_auth.hpp"

namespace stampmq {

ConfigAuth::ConfigAuth(ConfigPtr conf) : Authenticator(conf)
{
    if (!(*conf)["users"]) {
        throw FatalException("no users in Config.");
    }
}

PrincipalPtr ConfigAuth::Authenticate(const std::string& username, const std::string& pass)
{
    const YAML::Node& users = (*config_)["users"];
    const YAML::Node& user = users[username];
    if (!user) {
        throw ConnectionException("missing user "+username);
    }
    std::string config_pass = user["password"].as<std::string>("");
    if (config_pass.empty()) {
        throw ConnectionException("Config is missing password for user "+username);
    }
    if (!pass.compare(config_pass)) {
        PrincipalPtr p = std::make_shared<Principal>();
        p->set_name(username);
        p->set_authenticated(true);
        const YAML::Node& roles = user["roles"];
        if (roles) {
            for (auto i = roles.begin(); i != roles.end(); ++i) {
                std::string role = i->as<std::string>("");
                p->AddRole(role);
            }
        }
        return p;
    } else {
        throw ConnectionException("wrong password.");
    }
}

}
