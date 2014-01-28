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

#include <memory>

#include <log4cxx/logger.h>

#include "connection.hpp"
#include "connection_exception.hpp"
#include "nack_frame.hpp"

namespace stampmq {

using namespace log4cxx;

LoggerPtr ack_frame_logger(Logger::getLogger("stampmq.NackFrame"));

void NackFrame::Process(ConnectionPtr conn)
{
    //LOG4CXX_DEBUG (ack_frame_logger, "ACK frames not supported yet.");

    if (headers_.count("message-id") != 1)
        throw ConnectionException("message-id header missing.");

    conn->Nack(headers_["message-id"]);
}

}
