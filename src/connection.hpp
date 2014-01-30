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

#ifndef STAMPMQ_CONNECTION_HPP
#define STAMPMQ_CONNECTION_HPP

#include <memory>
#include <map>
#include <queue>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "subscription_type.hpp"
#include "socket.hpp"
#include "frame.hpp"
#include "server.hpp"
#include "destination.hpp"
#include "connector.hpp"
#include "acl.hpp"
#include "principal.hpp"
#include "send_frame.hpp"

namespace stampmq {

enum CONN_STATE {
    none,
    headers,
    body,
    disconnected
};

class Frame;
typedef std::shared_ptr<Frame> FramePtr;
class MessageDelivery;
typedef std::shared_ptr<MessageDelivery> MessageDeliveryPtr;
class Delivery;
typedef std::shared_ptr<Delivery> DeliveryPtr;
class Destination;
typedef std::shared_ptr<Destination> DestinationPtr;
class Subscription;
typedef std::shared_ptr<Subscription> SubscriptionPtr;

typedef std::pair<ACL_ACTION, std::string> access_cache_entry;
typedef std::map<access_cache_entry, bool> access_cache;

#define INCOMING_BUFFER_LENGTH 16384

class Connection : public std::enable_shared_from_this<Connection>, boost::noncopyable {
public:
    Connection(boost::asio::io_service& io_service, Connector& Connector);
    ~Connection();
    virtual void Start();
    Server& server();
    std::string id();
    SubscriptionPtr Subscribe(const std::string& dstname, SUBSCRIPTION_TYPE type, const std::string& id);
    void Unsubscribe(const std::string& id);
    void QueueDelivery(DeliveryPtr delivery);
    void QueueMessage(SubscriptionPtr sub, FramePtr frame);
    void EraseAck(const std::string &msg_id);
    void Ack(const std::string &msg_id);
    void QueueFrame(FramePtr frame);
    void SendFrame(FramePtr frame);
    std::string NewUuid();
    void Disconnect();
    PrincipalPtr Authenticate(std::string user, std::string pass);
    bool IsAuthenticated();
    bool IsAllowed(const std::string &destination, ACL_ACTION action);
    virtual Socket& get_socket();
protected:
    void ProcessQueue();
    SubscriptionPtr GetSubscriptionById(const std::string &subscription_id);
    void InvalidateAccessCache();
    boost::asio::io_service& io_service_;
    PrincipalPtr user_;
    Connector& connector_;
    void DoRead();
    std::queue<DeliveryPtr> send_queue_;
    boost::shared_mutex send_queue_mtx_;
    bool sending_;
    boost::asio::streambuf sendbuf_;
    char netbuf_[INCOMING_BUFFER_LENGTH];
    std::string msgbuf_;
    CONN_STATE state_;
    FramePtr frame_;
    std::map<std::string, SubscriptionPtr> subscriptions_;
    std::map<std::string, SubscriptionPtr> subscriptions_by_destination_;
    boost::shared_mutex subscriptions_mtx_;
    access_cache access_cache_;
    boost::shared_mutex access_cache_mtx_;
    boost::signals2::connection invalidate_connection_;
    std::string id_;
    std::map<std::string, MessageDeliveryPtr> ack_map_;
    boost::uuids::uuid uuidgen_;
};

typedef std::shared_ptr<Connection> ConnectionPtr;

} // namespace stampmq

#endif
