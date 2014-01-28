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
#include <boost/bind.hpp>
#include <log4cxx/logger.h>

#include "thread_pool.hpp"

namespace stampmq {

using namespace log4cxx;

LoggerPtr thread_pool_logger(Logger::getLogger("stampmq.ThreadPool"));

ThreadPool::ThreadPool(boost::asio::io_service& service) : service_(service)
{
    hardware_concurrency_ = size_ = boost::thread::hardware_concurrency();
}

void ThreadPool::set_size(unsigned int size)
{
    size_ = size;
}

void ThreadPool::Run()
{
    LOG4CXX_INFO(thread_pool_logger, "starting thread pool.");
    if (size_ == hardware_concurrency_) {
        LOG4CXX_INFO(thread_pool_logger, "thread pool size equal to hardware concurrency (" << hardware_concurrency_ << ")");
    } else if (size_ < hardware_concurrency_) {
        LOG4CXX_INFO(thread_pool_logger, "thread pool size (" << size_ << ") less than hardware concurrency (" << hardware_concurrency_ << ")");
    } else if (size_ > hardware_concurrency_) {
        LOG4CXX_INFO(thread_pool_logger, "thread pool size (" << size_ << ") greater than hardware concurrency (" << hardware_concurrency_ << ")");
    }
    for (std::size_t i = 0; i < size_; ++i) {
        group_.create_thread(boost::bind(&boost::asio::io_service::run, &service_));
    }
    group_.join_all();
}

}
