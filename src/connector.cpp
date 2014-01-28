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

#include <boost/asio.hpp>

#include "server.hpp"
#include "connection.hpp"
#include "connector.hpp"

namespace stampmq {

using boost::asio::ip::tcp;

Connector::Connector(boost::asio::io_service& io_service, const tcp::endpoint& endpoint, Server& server) : io_service_(io_service), server_(server), connections_(), acceptor_(io_service, endpoint)
{
}

Server& Connector::server()
{
    return server_;
}

void Connector::CloseAccept()
{
    acceptor_.close();
}

void Connector::AddConnection(ConnectionPtr conn)
{
    boost::unique_lock<boost::shared_mutex> lock;
    connections_.insert(conn);
}

void Connector::Disconnect(ConnectionPtr conn)
{
    boost::unique_lock<boost::shared_mutex> lock(connections_mtx_);
    connections_.erase(conn);
}

void Connector::DisconnectAll()
{
    boost::shared_lock<boost::shared_mutex> lock(connections_mtx_);
    std::set<ConnectionPtr> tmp(connections_);
    lock.unlock();
    for (auto iter = tmp.begin(); iter != tmp.end(); ++iter) {
        (*iter)->Disconnect();
    }
}
} // namespace stampmq
