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

#ifndef STAMPMQ_PLAIN_CONNECTOR_HPP
#define STAMPMQ_PLAIN_CONNECTOR_HPP

#include <memory>

#include <boost/asio.hpp>
#include <boost/utility.hpp>

#include "server.hpp"
#include "connector.hpp"

namespace stampmq {

using boost::asio::ip::tcp;

class Server;

class PlainConnector : public Connector {
public:
    PlainConnector(boost::asio::io_service& io_service,
                   const tcp::endpoint& endpoint, Server &server);
    Server& server();
    void Start();
protected:
    void DoAccept();
    tcp::socket socket_;
};

typedef std::shared_ptr<PlainConnector> PlainConnectorPtr;

} // namespace stampmq

#endif
