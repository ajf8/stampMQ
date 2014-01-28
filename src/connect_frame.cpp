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

#include "server.hpp"
#include "connection.hpp"
#include "connection_exception.hpp"
#include "connected_frame.hpp"
#include "connect_frame.hpp"

namespace stampmq {

void ConnectFrame::Process(ConnectionPtr conn)
{
    if (HeaderExists("login") && HeaderExists("passcode")) {
        conn->Authenticate(headers_["login"], headers_["passcode"]);
        if (conn->IsAuthenticated()) {
            auto rsp = std::make_shared<ConnectedFrame>();
            rsp->SetHeader("Server", conn->server().signature());
            rsp->SetHeader("heart-beat", "0,0");
            rsp->SetHeader("session", conn->id());
            rsp->SetHeader("version", "1.0");
            conn->QueueFrame(rsp);
        } else {
            throw ConnectionException("invalid login.");
        }
    } else {
        throw ConnectionException("login and passcode headers required.");
    }
}

bool ConnectFrame::RequiresAuthentication()
{
    return false;
}

}
