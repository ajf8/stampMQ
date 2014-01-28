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

#ifndef STAMPMQ_SSL_CONNECTOR_HPP
#define STAMPMQ_SSL_CONNECTOR_HPP

#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/utility.hpp>

#include "ssl_socket.hpp"
#include "server.hpp"
#include "connector.hpp"

namespace stampmq {

using boost::asio::ip::tcp;

class Server;

class SslConnector : public Connector {
public:
    SslConnector(boost::asio::io_service& io_service,
                 const tcp::endpoint& endpoint, Server &server);
    Server& server();
    void Start();
    void set_ssl_password(const std::string &password);
    void set_cert_file(const std::string &file);
protected:
    std::string cert_pass_;
    std::string cert_file_;
    std::string ssl_password() const;
    void DoAccept();
    boost::asio::ssl::context context_;
    ssl_socket socket_;
};

typedef std::shared_ptr<SslConnector> SslConnectorPtr;

} // namespace stampmq

#endif
