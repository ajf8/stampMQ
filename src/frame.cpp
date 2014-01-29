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
#include <iostream>
#include <exception>

#include "connection.hpp"
#include "fatal_exception.hpp"
#include "frame.hpp"

namespace stampmq {

Frame::Frame() : headers_(), to_receive_(-1)
{

}

std::string Frame::frame_name()
{
    throw FatalException("override Frame::frame_name()");
}

// TODO: should probably save duplicate headers in frame, only use first
// according to spec
void Frame::SetHeader(std::string hdkey, std::string hdval)
{
    headers_[hdkey] = hdval;
    if (!hdkey.compare("content-length")) {
        to_receive_ = atol(hdval.c_str());
    }
}

const std::string& Frame::GetHeader(const char *hdkey) {
    return headers_[hdkey];
}

void Frame::SetHeader(const char *hdkey, const char *hdval)
{
    SetHeader(std::string(hdkey), std::string(hdval));
}

headers_t& Frame::headers()
{
    return headers_;
}

bool Frame::HeaderExists(const char *key)
{
    return headers_.count(key) > 0;
}

void Frame::AppendToBody(char c)
{
    body_.push_back(c);
    if (to_receive_ > 0)
        to_receive_--;
}

const std::string& Frame::body() {
    return body_;
}

long Frame::to_receive()
{
    return to_receive_;
}

void Frame::Process(ConnectionPtr conn)
{
}

bool Frame::RequiresAuthentication()
{
    return true;
}

void Frame::Send(ConnectionPtr conn)
{
    conn->QueueFrame(shared_from_this());
}

} // namespace stampmq
