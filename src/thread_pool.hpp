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

#ifndef STAMPMQ_THREAD_POOL_HPP
#define STAMPMQ_THREAD_POOL_HPP

#include <list>

#include <boost/utility.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace stampmq {

class ThreadPool : boost::noncopyable {
public:
    ThreadPool(boost::asio::io_service& service);
    void set_size(unsigned int size);
    void Run();
protected:
    boost::asio::io_service& service_;
    std::list<boost::thread> threads_;
    unsigned int size_;
    unsigned int hardware_concurrency_;
    boost::thread_group group_;
};

}

#endif
