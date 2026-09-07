#include "orders.hpp"

OrderId OrderBook::add(Side side, Price price, Quantity quantity) {
    Order order{next_id_++, side, price, quantity, std::time(nullptr)};
    auto &levels = side == Side::Bid ? bids_ : asks_;
    auto &orders = levels[price].orders;

    auto it = orders.insert(orders.end(), order);
    orders_by_id_.emplace(order.id, it);

    last_update_ = order.date;
    return order.id;
}

bool OrderBook::cancel(OrderId id) {
    auto it = orders_by_id_.find(id);
    if (it == orders_by_id_.end()) {
        return false;
    }

    auto order_it = it->second;
    auto &levels = order_it->side == Side::Bid ? bids_ : asks_;
    auto level_it = levels.find(order_it->price);

    if(level_it != levels.end()) {
        level_it->second.orders.erase(order_it);
        if (level_it->second.orders.empty()) {
            levels.erase(level_it);
        }
    }

    orders_by_id_.erase(it);
    last_update_ = std::time(nullptr);
    return true;
}

const Order *OrderBook::find(OrderId id) const {
    auto it = orders_by_id_.find(id);
    return it == orders_by_id_.end() ? nullptr : &*(it->second);
}
