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

#include "frame.hpp"
#include "connect_frame.hpp"
#include "disconnect_frame.hpp"
#include "send_frame.hpp"
#include "subscribe_frame.hpp"
#include "unsubscribe_frame.hpp"
#include "unknown_frame.hpp"
#include "ack_frame.hpp"
#include "frame_factory.hpp"

namespace stampmq {

std::shared_ptr<Frame> FrameFactory::Create(std::string &name)
{
    if (!name.compare("CONNECT")) {
        return std::make_shared<ConnectFrame>();
    } else if (!name.compare("SEND")) {
        return std::make_shared<SendFrame>();
    } else if (!name.compare("SUBSCRIBE")) {
        return std::make_shared<SubscribeFrame>();
    } else if (!name.compare("UNSUBSCRIBE")) {
        return std::make_shared<unSubscribeFrame>();
    } else if (!name.compare("DISCONNECT")) {
        return std::make_shared<DisconnectFrame>();
    } else if (!name.compare("ACK") || !name.compare("NACK")) {
        return std::make_shared<AckFrame>();
    }
    return std::make_shared<UnknownFrame>(name);
}

}
