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
#include <log4cxx/logger.h>

#include "connection.hpp"
#include "ssl_connection.hpp"

namespace stampmq {

using namespace log4cxx;

LoggerPtr ssl_conn_logger(Logger::getLogger("stampmq.SslConnection"));

SslConnection::SslConnection(boost::asio::io_service& io_service, boost::asio::ssl::context& context, Connector& connector) :
    Connection(io_service, connector), socket_(io_service, context)
{
}

void SslConnection::Start()
{
    auto self(shared_from_this());
    socket_.async_handshake([this, self](boost::system::error_code ec) {
        if (!ec) {
            DoRead();
        }
    });

    /*socket_.async_handshake(boost::asio::ssl::stream_base::server,
        boost::bind(&SslConnection::handle_handshake, this,
          boost::asio::placeholders::error));*/
}

Socket& SslConnection::get_socket()
{
    return socket_;
}

ssl_socket::lowest_layer_type& SslConnection::accept_socket()
{
    return socket_.accept_socket();
}

} // namespace stampmq
