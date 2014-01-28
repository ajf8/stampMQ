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

#include "connection.hpp"
#include "error_frame.hpp"
#include "unknown_frame.hpp"

namespace stampmq {

UnknownFrame::UnknownFrame(std::string& frame_name) : ErrorFrame(std::string("unknown frame ")+frame_name)
{
}

void UnknownFrame::Process(ConnectionPtr conn)
{
    conn->QueueFrame(shared_from_this());
}

}
