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

#ifndef STAMPMQ_SOCKET_HPP
#define STAMPMQ_SOCKET_HPP

#include <functional>
#include <exception>

#include <boost/asio.hpp>
#include <boost/utility.hpp>

namespace stampmq {

class Socket : boost::noncopyable {
public:
    virtual void async_write(boost::asio::streambuf& buf, std::function<void(const boost::system::error_code&,std::size_t bytes_transferred)> writeHandler) = 0;
    virtual void async_write(boost::asio::mutable_buffers_1, std::function<void(const boost::system::error_code&,std::size_t bytes_transferred)> writeHandler) = 0;
    virtual void async_accept(std::function<void(const boost::system::error_code&)> acceptHandler);
    virtual void async_read_some(boost::asio::mutable_buffers_1 buf, std::function<void(const boost::system::error_code&,std::size_t bytes_transferred)> readHandler) = 0;
    virtual void close() = 0;
};

}

#endif
