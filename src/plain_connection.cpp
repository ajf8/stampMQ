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

#include "plain_socket.hpp"
#include "plain_connection.hpp"
#include "connector.hpp"

namespace stampmq {

PlainConnection::PlainConnection(boost::asio::ip::tcp::socket socket, boost::asio::io_service& io_service, Connector &connector) : Connection(io_service, connector), socket_(socket)
{
}

Socket& PlainConnection::get_socket()
{
    return socket_;
}

boost::asio::ip::tcp::socket& PlainConnection::get_accept_socket()
{
    return socket_.accept_socket();
}

}
