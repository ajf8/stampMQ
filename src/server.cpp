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
#include <exception>
#include <list>

#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <log4cxx/logger.h>
#include <yaml-cpp/yaml.h>

#include "server.hpp"
#include "config.hpp"
#include "hub.hpp"
#include "connector.hpp"
#include "plain_connector.hpp"
#include "ssl_connector.hpp"
#include "config_auth.hpp"
#include "fatal_exception.hpp"
#include "yaml_configurator.hpp"

namespace stampmq {

using boost::asio::ip::tcp;
using namespace log4cxx;

LoggerPtr server_logger(Logger::getLogger("stampmq.Server"));

Server::Server(boost::program_options::variables_map& cmdline) :
    signature_(std::string(PACKAGE_NAME) + "/" + PACKAGE_VERSION),
    cmdline_(cmdline), io_service_(), signals_(io_service_), connectors_(), tpool_(io_service_)
{
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

    config_ = std::make_shared<Config>();

    config_->Load(FindConfigFile());

    YamlConfigurator::configure(config_);

    hub_ = new Hub();
}

std::string Server::signature()
{
    return signature_;
}

std::string Server::FindConfigFile()
{
    if (cmdline_.count("conf") == 1) {
        std::string conf_arg = cmdline_["conf"].as<std::string>();
        if (boost::filesystem::is_regular_file(boost::filesystem::path(conf_arg))) {
            return conf_arg;
        } else {
            throw FatalException(std::string("config not found: ")+conf_arg);
        }
    }
    return std::string("/etc/stampmq/stampmq.yml");
}

void Server::Init()
{
    auth_ = std::make_shared<ConfigAuth>(config_);
    acl_ = config_->MakeAcl();

    const YAML::Node& connectors = (*config_)["connectors"];
    if (!connectors || connectors.size() == 0) {
        throw FatalException("no connectors defined.");
    }

    for (auto iter = connectors.begin(); iter != connectors.end();
            ++iter) {
        int port = (*iter)["port"].as<int>();

        tcp::endpoint endpoint(tcp::v4(), port);

        if ((*iter)["ssl"]) {
            SslConnectorPtr conn = std::make_shared<SslConnector>(io_service_, endpoint, *this);
            std::string cert_file = (*iter)["ssl"]["cert_file"].as<std::string>("");
            if (!cert_file.empty()) {
                conn->set_cert_file(cert_file);
            }
            std::string cert_password = (*iter)["ssl"]["cert_password"].as<std::string>("");
            if (!cert_password.empty()) {
                conn->set_ssl_password(cert_password);
            }
            conn->Start();
            connectors_.push_back(conn);
        } else {
            PlainConnectorPtr conn = std::make_shared<PlainConnector>(io_service_, endpoint, *this);
            conn->Start();
            connectors_.push_back(conn);
        }
    }
}

void Server::CloseAcceptors()
{
    for (auto iter = connectors_.begin(); iter != connectors_.end(); ++iter) {
        (*iter)->CloseAccept();
    }
}

void Server::DisconnectAll()
{
    for (auto iter = connectors_.begin(); iter != connectors_.end(); iter++) {
        (*iter)->DisconnectAll();
    }
}

int Server::Run()
{
    try {
        Init();
        signals_.async_wait(
        [this](boost::system::error_code ec, int signo) {
            LOG4CXX_ERROR(server_logger, "received signal " << signo << ". shutting down.");
            CloseAcceptors();
            DisconnectAll();
        });
        tpool_.Run();
    } catch (std::exception &ex) {
        LOG4CXX_ERROR(server_logger, ex.what());
        return 1;
    }
    return 0;
}

Hub& Server::hub()
{
    return *hub_;
}

AclChainPtr Server::acl_chain()
{
    return acl_;
}

ConfigPtr Server::config()
{
    return config_;
}

PrincipalPtr Server::Authenticate(std::string user, std::string pass)
{
    LOG4CXX_DEBUG(server_logger, "authenticating " << user);
    PrincipalPtr p = auth_->Authenticate(user, pass);
    LOG4CXX_DEBUG(server_logger, "isAuthenticated=" << p->IsAuthenticated());
    LOG4CXX_DEBUG(server_logger, "roles: " << p->RolesAsString());
    return p;
}

} // namespace stampmq
