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
#include "subscription.hpp"
#include "connection_exception.hpp"
#include "subscribe_frame.hpp"

namespace stampmq {

std::string SubscribeFrame::frame_name()
{
    return std::string("SUBSCRIBE");
}

SUBSCRIPTION_TYPE SubscribeFrame::Ack() {
    if (!HeaderExists("ack")) {
        return AUTO;
    }
    const std::string& ack = headers_["ack"];
    if (!ack.compare("client-individual")) {
        return CLIENT_INDIVIDUAL;
    } else if (!ack.compare("auto")) {
        return AUTO;
    } else if (!ack.compare("client")) {
        return CLIENT;
    } else {
        throw ConnectionException("invalid ack mode");
    }
}

void SubscribeFrame::Process(ConnectionPtr conn)
{
    if (!HeaderExists("destination")) {
        throw ConnectionException("destination header required.");
    }
    if (!HeaderExists("id")) {
        throw ConnectionException("id header required.");
    }
    const std::string& dest = headers_["destination"];
    const std::string& id = headers_["id"];
    if (!conn->IsAllowed(dest, read)) {
        throw ConnectionException("not authorized to read destination: " + dest);
    }
    conn->Subscribe(dest, Ack(), id);
}

}
