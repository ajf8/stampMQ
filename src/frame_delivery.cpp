#include "connection.hpp"
#include "frame_delivery.hpp"

namespace stampmq {

FrameDelivery::FrameDelivery(FramePtr frame, ConnectionPtr connection) : frame_(frame), connection_(connection) {

}

bool FrameDelivery::Deliver() {
    connection_->SendFrame(frame_);
    return true;
}

}
