#include <ctime>
#include <iostream>

#include "orders.hpp"

int main() {
    using namespace std;
    using namespace std::chrono;
    OrderBook book("AAPL");

    OrderId b1 = book.add(Side::Bid, 99.0, 100);
    book.add(Side::Bid, 98.5, 200);
    book.add(Side::Ask, 100.5, 75);
    book.add(Side::Ask, 101.0, 150);
    auto time = book.last_update();
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    Order order1 = *book.find(b1);

    std::cout << "OrderBook demo: " << book.symbol() << '\n'
              << "Best bid: " << book.best_bid() << '\n'
              << "Best ask: " << book.best_ask() << '\n'
              << "Orders: " << book.size() << '\n'
              << "Order 1 id: " << order1.id << '\n'
              << "Order 1 price: " << order1.price << '\n'
              << "Order 1 quantity: " << order1.quantity << '\n'
              << "Last updated: " << buf << '\n';
    return 0;
}
