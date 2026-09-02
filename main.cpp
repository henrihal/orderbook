#include <iostream>

#include "orders.hpp"

int main() {
    OrderBook book("AAPL");

    book.add(Side::Bid, 99.0, 100);
    book.add(Side::Bid, 98.5, 200);
    book.add(Side::Ask, 100.5, 75);
    book.add(Side::Ask, 101.0, 150);

    std::cout << "OrderBook demo: " << book.symbol() << '\n'
              << "Best bid: " << book.best_bid() << '\n'
              << "Best ask: " << book.best_ask() << '\n'
              << "Orders: " << book.size() << '\n';
    return 0;
}
