#ifndef STAMPMQ_FRAME_DELIVERY_HPP
#define STAMPMQ_FRAME_DELIVERY_HPP

#include <memory>

#include "delivery.hpp"

namespace stampmq {

class Frame;
typedef std::shared_ptr<Frame> FramePtr;
class Connection;
typedef std::shared_ptr<Connection> ConnectionPtr;

class FrameDelivery : public Delivery {
public:
    FrameDelivery(FramePtr frame, ConnectionPtr connection);
    bool Deliver();
protected:
    FramePtr frame_;
    ConnectionPtr connection_;
};

typedef std::shared_ptr<FrameDelivery> FrameDeliveryPtr;

}

#endif
