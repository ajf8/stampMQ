#ifndef STAMPMQ_SUBSCRIPTION_HPP
#define STAMPMQ_SUBSCRIPTION_HPP

#include <memory>
#include <string>
#include <queue>

#include <boost/utility.hpp>

#include "connection.hpp"
#include "subscription_type.hpp"
//#include "destination.hpp"

namespace stampmq {

class Destination;
typedef std::shared_ptr<Destination> DestinationPtr;
class Connection;
typedef std::shared_ptr<Connection> ConnectionPtr;
class MessageDelivery;
typedef std::shared_ptr<MessageDelivery> MessageDeliveryPtr;

class Subscription : public std::enable_shared_from_this<Subscription>, boost::noncopyable {
public:
    Subscription(DestinationPtr dest, ConnectionPtr connection, SUBSCRIPTION_TYPE type, const std::string& id);
    DestinationPtr destination();
    ConnectionPtr connection();
    std::string id();
    SUBSCRIPTION_TYPE type();
    void Disconnect();
    void RecordForCumulativeAck(MessageDeliveryPtr delivery);
    void CumulativeAck(const std::string &msg_id);
protected:
    DestinationPtr destination_;
    ConnectionPtr connection_;
    SUBSCRIPTION_TYPE type_;
    std::string id_;
    std::queue<MessageDeliveryPtr> ack_list_;
};

typedef std::shared_ptr<Subscription> SubscriptionPtr;

}

#endif
