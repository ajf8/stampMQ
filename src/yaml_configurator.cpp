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

#include <log4cxx/helpers/properties.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/logmanager.h>

#include "fatal_exception.hpp"
#include "config.hpp"

#include "yaml_configurator.hpp"

namespace stampmq {

using namespace log4cxx;
using namespace log4cxx::spi;
using namespace log4cxx::helpers;

void YamlConfigurator::configure(ConfigPtr config)
{
    Properties props;
    YAML::Node logging_config = (*config)["logging"];
    if (!logging_config || logging_config.Type() != YAML::NodeType::Map) {
        throw FatalException("config missing logging section.");
    }
    for (auto iter = logging_config.begin(); iter != logging_config.end(); ++iter) {
        std::string key = iter->first.as<std::string>("");
        std::string value = iter->second.as<std::string>("");
        if (!key.empty() && !value.empty()) {
            props.put(key, value);
        }
    }
    PropertyConfigurator().doConfigure(props, LogManager::getLoggerRepository());
}

}
