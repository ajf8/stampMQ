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

#include <string>

#include "server.hpp"
#include "connection.hpp"
#include "connection_exception.hpp"
#include "unsubscribe_frame.hpp"

namespace stampmq {

std::string unSubscribeFrame::frame_name()
{
    return std::string("UNSUBSCRIBE");
}

void unSubscribeFrame::Process(ConnectionPtr conn)
{
    if (!HeaderExists("destination")) {
        throw ConnectionException("Destination header required.");
    }
    std::string dest = headers_["destination"];
    conn->Unsubscribe(dest);
}

}
