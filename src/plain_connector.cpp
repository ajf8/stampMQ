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

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>

#include "plain_connection.hpp"
#include "server.hpp"
#include "connector.hpp"
#include "plain_connector.hpp"

namespace stampmq {

using boost::asio::ip::tcp;

PlainConnector::PlainConnector(boost::asio::io_service& io_service,
                               const tcp::endpoint& endpoint, Server& server) :
    Connector(io_service, endpoint, server), socket_(io_service)
{
}

void PlainConnector::DoAccept()
{
    PlainConnectionPtr conn = std::make_shared<PlainConnection>(std::move(socket_), io_service_, *this);
    acceptor_.async_accept(conn->get_accept_socket(), [this, conn](boost::system::error_code ec) {
        if (!ec) {
            AddConnection(conn);
            conn->Start();
            DoAccept();
        }
    });
}

void PlainConnector::Start()
{
    DoAccept();
}

} // namespace stampmq
