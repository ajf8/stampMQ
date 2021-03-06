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

#ifndef STAMPMQ_CONNECT_FRAME_HPP
#define STAMPMQ_CONNECT_FRAME_HPP

#include "frame.hpp"

namespace stampmq {

class ConnectFrame : public Frame {
public:
    void Process(ConnectionPtr conn);
    bool RequiresAuthentication();
};

} // namespace stampmq

#endif // STAMPMQ_CONNECT_FRAME_HPP
