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

#include <functional>
#include <exception>

#include <boost/asio.hpp>

#include "ssl_socket.hpp"

namespace stampmq {

SslSocket::SslSocket(boost::asio::io_service& service, boost::asio::ssl::context &ctx) : socket_(service, ctx)
{

}

void SslSocket::async_write(boost::asio::streambuf& buf, std::function<void(const boost::system::error_code&,std::size_t bytes_transferred)> writeHandler)
{
    boost::asio::async_write(socket_, buf, writeHandler);
}

void SslSocket::async_write(boost::asio::mutable_buffers_1 buf, std::function<void(const boost::system::error_code&,std::size_t bytes_transferred)> writeHandler)
{
    boost::asio::async_write(socket_, buf, writeHandler);
}

void SslSocket::async_handshake(std::function<void(const boost::system::error_code&)> handler)
{
    socket_.async_handshake(boost::asio::ssl::stream_base::server, handler);
}

void SslSocket::async_read_some(boost::asio::mutable_buffers_1 buf, std::function<void(const boost::system::error_code&,std::size_t bytes_transferred)> readHandler)
{
    socket_.async_read_some(buf, readHandler);
}

ssl_socket::lowest_layer_type& SslSocket::accept_socket()
{
    return socket_.lowest_layer();
}

void SslSocket::close()
{
    socket_.lowest_layer().close();
}

}
