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

#ifndef STAMPMQ_SERVER_HPP
#define STAMPMQ_SERVER_HPP

#include <memory>

#include <boost/utility.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/asio.hpp>

#include "config.h"
#include "config.hpp"
//#include "hub.hpp"
#include "authenticator.hpp"
#include "acl_chain.hpp"
#include "thread_pool.hpp"
#include "connector.hpp"

namespace stampmq {

class Hub;

class Server : boost::noncopyable {
public:
    Server(boost::program_options::variables_map &cmdline);
    int Run();
    Hub& hub();
    AclChainPtr acl_chain();
    ConfigPtr config();
    PrincipalPtr Authenticate(std::string user, std::string pass);
    std::string signature();
protected:
    void CloseAcceptors();
    void DisconnectAll();
    std::string signature_;
    std::string FindConfigFile();
    boost::program_options::variables_map cmdline_;
    void Init();
    boost::asio::io_service io_service_;
    boost::asio::signal_set signals_;
    std::list<ConnectorPtr> connectors_;
    Hub* hub_;
    ConfigPtr config_;
    AuthenticatorPtr auth_;
    AclChainPtr acl_;
    ThreadPool tpool_;
};

} // namespace stampmq

#endif
