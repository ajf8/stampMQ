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

#ifndef STAMPMQ_SEND_FRAME_HPP
#define STAMPMQ_SEND_FRAME_HPP

#include "frame.hpp"
//#include "subscription.hpp"

namespace stampmq {

class SendFrame : public Frame {
public:
    std::string frame_name();
    void Process(ConnectionPtr conn);
    const std::string& destination();
    const std::string& msg_id();
};

typedef std::shared_ptr<SendFrame> SendFramePtr;

} // namespace stampmq

#endif // STAMPMQ_CONNECT_FRAME_HPP
