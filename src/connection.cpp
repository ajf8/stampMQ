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

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <log4cxx/logger.h>

#include "socket.hpp"
#include "connection.hpp"
#include "server.hpp"
#include "connector.hpp"
#include "frame_factory.hpp"
#include "fatal_exception.hpp"
#include "connection_exception.hpp"
#include "principal.hpp"
#include "frame.hpp"
#include "hub.hpp"
#include "frame_delivery.hpp"
#include "message_delivery.hpp"

namespace stampmq {

using namespace log4cxx;

LoggerPtr conn_logger(Logger::getLogger("stampmq.Connection"));

Connection::Connection(boost::asio::io_service& io_service, Connector& connector)
    : io_service_(io_service), user_(new Principal()), connector_(connector), send_queue_(), state_(none)
{
    Server& srv = server();

    AclChainPtr aclc = srv.acl_chain();
    aclc->invalidated().connect(boost::bind(&Connection::InvalidateAccessCache, this));

    uuidgen_ = boost::uuids::random_generator()();
    id_ = NewUuid();
}

Connection::~Connection()
{
    LOG4CXX_DEBUG (conn_logger, "destroying connection")
    //Disconnect();
    Server& srv = server();
    AclChainPtr aclc = srv.acl_chain();
    aclc->invalidated().disconnect(invalidate_connection_);
}

std::string Connection::NewUuid()
{
    return boost::lexical_cast<std::string>(uuidgen_);
}

Server& Connection::server()
{
    return connector_.server();
}

std::string Connection::id()
{
    return id_;
}

SubscriptionPtr Connection::Subscribe(const std::string& dstname, SUBSCRIPTION_TYPE type, const std::string& id)
{
    LOG4CXX_DEBUG (conn_logger, "subscribing to " << dstname);
    SubscriptionPtr sub = server().hub().Subscribe(shared_from_this(), dstname, type, id);
    boost::unique_lock<boost::shared_mutex> lock(subscriptions_mtx_);
    subscriptions_[id] = sub;
    subscriptions_by_destination_[dstname] = sub;
    return sub;
}

SubscriptionPtr Connection::GetSubscriptionById(const std::string& id) {
    boost::shared_lock<boost::shared_mutex> lock(subscriptions_mtx_);
    if (subscriptions_.count(id) > 0) {
        return subscriptions_[id];
    } else {
        throw ConnectionException("subscription not found");
    }
}

void Connection::Unsubscribe(const std::string& id)
{
    boost::unique_lock<boost::shared_mutex> lock(subscriptions_mtx_);

    if (boost::starts_with(id, "/queue/") || boost::starts_with(id, "/topic/")) {
        if (subscriptions_by_destination_.count(id) > 0) {
            SubscriptionPtr sub = subscriptions_by_destination_[id];
            LOG4CXX_DEBUG (conn_logger, "unsubscribe dest=" << id);
            sub->Disconnect();
            subscriptions_.erase(sub->id());
            subscriptions_by_destination_.erase(id);
        } else {
            LOG4CXX_WARN (conn_logger, "unable to find subscription with ID: " << id);
        }
    } else {
        if (subscriptions_.count(id) > 0) {
            SubscriptionPtr sub = subscriptions_[id];
            LOG4CXX_DEBUG (conn_logger, "unsubscribe id=" << id << ", name=" << sub->destination()->name());
            subscriptions_[id]->Disconnect();
            subscriptions_.erase(id);
            subscriptions_by_destination_.erase(sub->destination()->name());
        } else {
            LOG4CXX_WARN (conn_logger, "unable to find subscription with ID: " << id);
        }
    }
}

void Connection::Start()
{
    DoRead();
}

void Connection::Disconnect()
{
    state_ = disconnected;
    boost::upgrade_lock<boost::shared_mutex> uplock(subscriptions_mtx_);
    for (std::map<std::string,SubscriptionPtr>::iterator iter = subscriptions_.begin(); iter != subscriptions_.end(); ++iter) {
        iter->second->Disconnect();
    }
    boost::upgrade_to_unique_lock<boost::shared_mutex> wlock(uplock);
    subscriptions_.clear();
    subscriptions_by_destination_.clear();
    connector_.Disconnect(shared_from_this());
    get_socket().close();
}

PrincipalPtr Connection::Authenticate(std::string user, std::string pass)
{
    return user_ = server().Authenticate(user, pass);
}

bool Connection::IsAuthenticated()
{
    return user_->IsAuthenticated();
}

void Connection::InvalidateAccessCache()
{
    boost::unique_lock<boost::shared_mutex> lock(access_cache_mtx_);
    access_cache_.clear();
}

bool Connection::IsAllowed(const std::string& destination, ACL_ACTION action)
{
    access_cache_entry cache_entry(action, destination);
    boost::upgrade_lock<boost::shared_mutex> uplock(access_cache_mtx_);
    if (access_cache_.count(cache_entry) == 1) {
        LOG4CXX_TRACE (conn_logger, "acl cache hit.")
        return access_cache_[cache_entry];
    }
    AclChainPtr aclc = server().acl_chain();
    bool result = aclc->IsAllowed(user_, destination, action);
    boost::upgrade_to_unique_lock<boost::shared_mutex> wlock(uplock);
    access_cache_[cache_entry] = result;
    return result;
}

void Connection::DoRead()
{
    auto self(shared_from_this());
    get_socket().async_read_some(boost::asio::buffer(netbuf_, sizeof(netbuf_)),
    [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            if (length == 0) {
                Disconnect();
                return;
            }
            for (unsigned int i = 0; i < length; i++) {
                char c = netbuf_[i];
                std::cout << c;
                if (state_ == body && c == '\0' && frame_->to_receive() < 1) {
                    state_ = none;
                    try {
                        if (frame_->RequiresAuthentication() && !IsAuthenticated()) {
                            throw ConnectionException("not authenticated.");
                        }
                        frame_->Process(shared_from_this());
                    } catch (ConnectionException& ex) {
                        LOG4CXX_ERROR (conn_logger, ex.msg())
                        ex.SendResponse(shared_from_this());
                    }
                    continue;
                }
                if (state_ == headers && c == '\n') {
                    if (msgbuf_.empty()) {
                        state_ = body;
                    } else {
                        // TODO: value can contain colons
                        std::vector< std::string > header;
                        boost::split(header, msgbuf_, boost::is_any_of(":"));
                        frame_->SetHeader(header[0], header[1]);
                        msgbuf_ = "";
                    }
                    continue;
                }
                if (state_ == none && c == '\n' && !msgbuf_.empty()) {
                    frame_ = FrameFactory::Create(msgbuf_);
                    LOG4CXX_DEBUG (conn_logger, "start read " << msgbuf_ << " frame")
                    msgbuf_ = "";
                    state_ = headers;
                    continue;
                }
                if (state_ == body) {
                    frame_->AppendToBody(c);
                } else {
                    msgbuf_ += c;
                }
            }
            DoRead();
        } else {
            Disconnect();
        }
    });
}

void Connection::ProcessQueue()
{
    if (state_ == disconnected) {
        sending_ = false;
        return;
    }
    DeliveryPtr delivery = send_queue_.front();
    send_queue_.pop();
    delivery->Deliver();
}

void Connection::QueueDelivery(DeliveryPtr delivery)
{
    if (state_ == disconnected) {
        return;
    }
    {
        //boost::unique_lock<boost::shared_mutex> lock(send_queue_mtx_);
        send_queue_.push(delivery);
    }
    if (sending_ == false) {
        sending_ = true;
        io_service_.post(boost::bind(&Connection::ProcessQueue, this));
    }
}

void Connection::QueueMessage(SubscriptionPtr sub, FramePtr frame) {
    const std::string& msg_id = frame->GetHeader("message-id");
    MessageDeliveryPtr delivery = std::make_shared<MessageDelivery>(msg_id, sub, frame);

    ack_map_[msg_id] = delivery;

    if (sub->type() == CLIENT)
        sub->RecordForCumulativeAck(delivery);

    QueueDelivery(delivery);
}

void Connection::Ack(const std::string &msg_id) {
    if (ack_map_.count(msg_id) != 1) {
        throw ConnectionException("unable to find frame for acknowledgement.");
    }

    MessageDeliveryPtr delivery = ack_map_[msg_id];
    SubscriptionPtr subscription = delivery->subscription();
    SUBSCRIPTION_TYPE type = subscription->type();

    if (type == CLIENT_INDIVIDUAL)
        delivery->Ack();
    else if (type == CLIENT)
        delivery->subscription()->CumulativeAck(msg_id);

    if (type == CLIENT || type == CLIENT_INDIVIDUAL)
        ack_map_.erase(msg_id);
}

void Connection::QueueFrame(FramePtr frame) {
    LOG4CXX_TRACE(conn_logger, "body:\n" << frame->body() << "\n");
    DeliveryPtr delivery = std::make_shared<FrameDelivery>(frame, shared_from_this());
    QueueDelivery(delivery);
}

void Connection::SendFrame(FramePtr frame) {
    std::ostream os(&sendbuf_);
    os << frame->frame_name() << "\n";
    headers_t& headers = frame->headers();
    for(headers_iterator_t iterator = headers.begin(); iterator != headers.end(); ++iterator) {
        os << iterator->first << ":" << iterator->second << "\n";
    }
    os << "\n" << frame->body() << '\0' << "\n";
    auto self(shared_from_this());
    get_socket().async_write(sendbuf_,
    [this, self](boost::system::error_code ec, std::size_t length) {
        if (ec || length <= 0) {
            sending_ = false;
            Disconnect();
        } else {
            //boost::shared_lock<boost::shared_mutex> lock(send_queue_mtx_);
            if (send_queue_.size() > 0) {
                io_service_.post(boost::bind(&Connection::ProcessQueue, this));
            } else {
                sending_ = false;
            }
        }
    });
}

Socket& Connection::get_socket()
{
    throw FatalException("socket() not overridden");
}

} // namespace stampmq
