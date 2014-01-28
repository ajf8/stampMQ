#include <string>

#include "destination.hpp"
#include "connection_exception.hpp"
#include "message_delivery.hpp"
#include "subscription.hpp"

namespace stampmq {

Subscription::Subscription(DestinationPtr destination, ConnectionPtr connection, SUBSCRIPTION_TYPE type, const std::string& id) : destination_(destination), connection_(connection), type_(type), id_(id) {

}

DestinationPtr Subscription::destination() {
    return destination_;
}

ConnectionPtr Subscription::connection() {
    return connection_;
}

std::string Subscription::id() {
    return id_;
}

SUBSCRIPTION_TYPE Subscription::type() {
    return type_;
}

void Subscription::Disconnect() {
    destination_->Unsubscribe(shared_from_this());
}

void Subscription::RecordForCumulativeAck(MessageDeliveryPtr delivery) {
    ack_list_.push(delivery);
}

void Subscription::CumulativeAck(const std::string &msg_id) {
    while (!ack_list_.empty()) {
        MessageDeliveryPtr delivery = ack_list_.front();
        ack_list_.pop();
        delivery->Ack();
        if (delivery->id() == msg_id) {
            break;
        }
    }
}

}
