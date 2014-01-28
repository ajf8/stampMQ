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

#ifndef STAMPMQ_PLAIN_CONNECTION_HPP
#define STAMPMQ_PLAIN_CONNECTION_HPP

#include <boost/asio.hpp>

#include "plain_socket.hpp"
#include "connection.hpp"

namespace stampmq {

class PlainConnection : public Connection {
public:
    PlainConnection(boost::asio::ip::tcp::socket socket, boost::asio::io_service& io_service, Connector &connector);
    Socket& get_socket();
    boost::asio::ip::tcp::socket& get_accept_socket();
protected:
    PlainSocket socket_;
};

typedef std::shared_ptr<PlainConnection> PlainConnectionPtr;

}

#endif
