#ifndef STAMPMQ_DELIVERY_HPP
#define STAMPMQ_DELIVERY_HPP

#include <memory>

namespace stampmq {

class Delivery;
typedef std::shared_ptr<Delivery> DeliveryPtr;

class Delivery {
public:
    virtual bool Deliver() = 0;
};

}

#endif
