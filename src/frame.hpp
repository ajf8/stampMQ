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

#ifndef STAMPMQ_FRAME_HPP
#define STAMPMQ_FRAME_HPP

#include <map>
#include <string>
#include <memory>

//#include "connection.hpp"

namespace stampmq {

typedef std::map<std::string,std::string> headers_t;
typedef headers_t::iterator headers_iterator_t;

class Connection;
typedef std::shared_ptr<Connection> ConnectionPtr;

class Frame : public std::enable_shared_from_this<Frame> {
public:
    Frame();
    virtual std::string frame_name();
    void SetHeader(std::string hdkey, std::string hdval);
    void SetHeader(const char *hdkey, const char *hdval);
    const std::string& GetHeader(const char *hdkey);
    headers_t& headers();
    bool HeaderExists(const char *key);
    std::string& body();
    void AppendToBody(char c);
    long to_receive();
    virtual void Process(ConnectionPtr conn);
    virtual bool RequiresAuthentication();
    void Send(ConnectionPtr conn);
protected:
    std::map<std::string,std::string> headers_;
    std::string body_;
    long to_receive_;
};

typedef std::shared_ptr<Frame> FramePtr;

} // namespace stampmq

#endif
