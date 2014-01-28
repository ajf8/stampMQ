#include <log4cxx/logger.h>

#include "subscription.hpp"
#include "message_delivery.hpp"

namespace stampmq {

using namespace log4cxx;

LoggerPtr message_delivery_logger(Logger::getLogger("stampmq.MessageDelivery"));

MessageDelivery::MessageDelivery(const std::string &id, SubscriptionPtr subscription, FramePtr frame) : id_(id), frame_(frame), subscription_(subscription) {

}

std::string MessageDelivery::id() {
    return id_;
}

FramePtr MessageDelivery::frame() {
    return frame_;
}

SubscriptionPtr MessageDelivery::subscription() {
    return subscription_;
}

bool MessageDelivery::Deliver() {
    subscription_->connection()->SendFrame(frame_);
    return true;
}

void MessageDelivery::Ack() {
    LOG4CXX_DEBUG (message_delivery_logger, "frame acknowledged.")
}

}
