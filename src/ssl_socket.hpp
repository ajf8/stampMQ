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

#ifndef STAMPMQ_SSL_SOCKET_HPP
#define STAMPMQ_SSL_SOCKET_HPP

#include <functional>
#include <exception>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "socket.hpp"

namespace stampmq {

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

class SslSocket : public Socket {
public:
    SslSocket(boost::asio::io_service& service, boost::asio::ssl::context &ctx);
    inline void async_write(boost::asio::streambuf& buf, std::function<void(const boost::system::error_code&,std::size_t bytes_transferred)> writeHandler);
    inline void async_write(boost::asio::mutable_buffers_1 buf, std::function<void(const boost::system::error_code&,std::size_t bytes_transferred)> writeHandler);
    void async_handshake(std::function<void(const boost::system::error_code&)> handler);
    inline void async_read_some(boost::asio::mutable_buffers_1 buf, std::function<void(const boost::system::error_code&,std::size_t bytes_transferred)> readHandler);
    ssl_socket::lowest_layer_type& accept_socket();
    void close();
protected:
    ssl_socket socket_;
};

}

#endif
