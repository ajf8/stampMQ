#ifndef STAMPMQ_MESSAGE_DELIVERY_HPP
#define STAMPMQ_MESSAGE_DELIVERY_HPP

#include <memory>

#include "delivery.hpp"

namespace stampmq {

class Frame;
typedef std::shared_ptr<Frame> FramePtr;
class Subscription;
typedef std::shared_ptr<Subscription> SubscriptionPtr;
class Delivery;
typedef std::shared_ptr<Delivery> DeliveryPtr;

class MessageDelivery : public Delivery {
public:
    MessageDelivery(const std::string &id, SubscriptionPtr sub, FramePtr frame);
    std::string id();
    FramePtr frame();
    SubscriptionPtr subscription();
    bool Deliver();
    void Ack();
protected:
    std::string id_;
    FramePtr frame_;
    SubscriptionPtr subscription_;
};

typedef std::shared_ptr<MessageDelivery> MessageDeliveryPtr;

}

#endif
