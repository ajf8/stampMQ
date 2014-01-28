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

#include <sys/time.h>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "server.hpp"
#include "connection.hpp"
#include "connection_exception.hpp"
#include "send_frame.hpp"
#include "hub.hpp"

namespace stampmq {

using namespace boost::posix_time;

std::string SendFrame::frame_name()
{
    return std::string("MESSAGE");
}

void SendFrame::Process(ConnectionPtr conn)
{
    if (!HeaderExists("destination")) {
        throw ConnectionException("destination header missing.");
    }
    std::string& destination = headers_["destination"];
    if (!conn->IsAllowed(destination, write)) {
        throw ConnectionException("not allowed to send to destination: "+destination);
    }
    struct timeval t;
    gettimeofday(&t, NULL);

    boost::uuids::uuid uuid = boost::uuids::random_generator()();

    headers_["message-id"] = boost::lexical_cast<std::string>(uuid);
    headers_["expires"] = "0";
    headers_["priority"] = "4";
    headers_["timestampmq"] = boost::lexical_cast<std::string>(t.tv_sec);

    conn->server().hub().Send(destination, shared_from_this());
}

const std::string& SendFrame::destination() {
    return headers_["destination"];
}

const std::string& SendFrame::msg_id() {
    return headers_["message-id"];
}

}
