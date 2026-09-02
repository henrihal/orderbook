#include "orders.hpp"

#include <algorithm>

OrderId OrderBook::add(Side side, Price price, Quantity quantity) {
    Order order{next_id_++, side, price, quantity, std::time(nullptr)};
    auto &levels = side == Side::Bid ? bids_ : asks_;
    levels[price].orders.push_back(order);
    orders_by_id_[order.id] = {side, price};
    last_update_ = order.date;
    return order.id;
}

bool OrderBook::cancel(OrderId id) {
    auto it = orders_by_id_.find(id);
    if (it == orders_by_id_.end()) {
        return false;
    }

    OrderLocation loc = it->second;
    auto &levels = loc.side == Side::Bid ? bids_ : asks_;
    auto level_it = levels.find(loc.price);
    if (level_it != levels.end()) {
        auto &orders = level_it->second.orders;
        auto order_it = std::find_if(orders.begin(), orders.end(), [id](const Order &o) { return o.id == id; });
        if (order_it != orders.end()) {
            orders.erase(order_it);
        }
        if (orders.empty()) {
            levels.erase(level_it);
        }
    }

    orders_by_id_.erase(it);
    last_update_ = std::time(nullptr);
    return true;
}

const Order *OrderBook::find(OrderId id) const {
    auto it = orders_by_id_.find(id);
    if (it == orders_by_id_.end()) {
        return nullptr;
    }

    OrderLocation loc = it->second;
    const auto &levels = loc.side == Side::Bid ? bids_ : asks_;
    auto level_it = levels.find(loc.price);
    if (level_it == levels.end()) {
        return nullptr;
    }

    for (const Order &order : level_it->second.orders) {
        if (order.id == id) {
            return &order;
        }
    }
    return nullptr;
}
