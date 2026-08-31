#include <cstdint>
#include <ctime>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

using Price = double;
using Quantity = double;
using OrderId = std::int64_t;

enum class Side : bool { Bid, Ask };

struct Order {
    OrderId id;
    Side side;
    Price price;
    Quantity quantity;
    std::time_t date;
};

class OrderBook {
  public:
    explicit OrderBook(std::string sym) : symbol_(std::move(sym)) {}

    void add(const Order &o);
    bool cancel(OrderId id);

    Price best_bid() const { return bids_.empty() ? 0.0 : bids_.rbegin()->first; }
    Price best_ask() const { return asks_.empty() ? 0.0 : asks_.begin()->first; }
    std::size_t size() const { return bids_.size() + asks_.size(); }

    const std::string &symbol() const { return symbol_; }
    std::time_t last_update() const { return last_update_; }

  private:
    struct Level {
        std::vector<Order> orders;
    };

    std::string symbol_;
    std::map<Price, Level> bids_;
    std::map<Price, Level> asks_;
    std::unordered_map<OrderId, std::pair<Side, Price>> id_index_;
    std::time_t last_update_;
};
