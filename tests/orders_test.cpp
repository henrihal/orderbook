#include "orders.hpp"

#include <gtest/gtest.h>

#include <vector>

namespace {

class OrderBookTest : public ::testing::Test {
  protected:
    OrderBook book{"AAPL"};

    std::vector<OrderId> populate() {
        return {book.add(Side::Bid, 99.0, 100),
                book.add(Side::Bid, 99.0, 50),
                book.add(Side::Bid, 98.5, 200),
                book.add(Side::Ask, 100.5, 75),
                book.add(Side::Ask, 101.0, 150)};
    }
};

TEST_F(OrderBookTest, FreshBookHasNoBestsOrOrders) {
    EXPECT_DOUBLE_EQ(book.best_bid(), 0.0);
    EXPECT_DOUBLE_EQ(book.best_ask(), 0.0);
    EXPECT_EQ(book.size(), 0u);
}

TEST_F(OrderBookTest, AddAssignsSequentialIds) {
    const std::vector<OrderId> expected{1, 2, 3, 4, 5};
    EXPECT_EQ(populate(), expected);
}

TEST_F(OrderBookTest, AddUpdatesBestsAndSize) {
    populate();
    EXPECT_DOUBLE_EQ(book.best_bid(), 99.0);
    EXPECT_DOUBLE_EQ(book.best_ask(), 100.5);
    EXPECT_EQ(book.size(), 5u);
}

TEST_F(OrderBookTest, FindReturnsOrderFields) {
    const auto ids = populate();
    const Order *order = book.find(ids[0]);
    ASSERT_NE(order, nullptr);
    EXPECT_EQ(order->side, Side::Bid);
    EXPECT_DOUBLE_EQ(order->price, 99.0);
    EXPECT_DOUBLE_EQ(order->quantity, 100.0);
}

TEST_F(OrderBookTest, FindUnknownIdReturnsNull) {
    EXPECT_EQ(book.find(999), nullptr);
}

TEST_F(OrderBookTest, CancelRemovesOrder) {
    const auto ids = populate();
    EXPECT_TRUE(book.cancel(ids[0]));
    EXPECT_EQ(book.size(), 4u);
    EXPECT_DOUBLE_EQ(book.best_bid(), 99.0);
    EXPECT_EQ(book.find(ids[0]), nullptr);
}

TEST_F(OrderBookTest, CancelDrainsPriceLevels) {
    const auto ids = populate();
    book.cancel(ids[0]);
    book.cancel(ids[1]);
    EXPECT_DOUBLE_EQ(book.best_bid(), 98.5);
    EXPECT_EQ(book.size(), 3u);
    book.cancel(ids[3]);
    EXPECT_DOUBLE_EQ(book.best_ask(), 101.0);
    EXPECT_EQ(book.size(), 2u);
}

TEST_F(OrderBookTest, CancelUnknownIdReturnsFalse) {
    populate();
    EXPECT_FALSE(book.cancel(999));
    EXPECT_EQ(book.size(), 5u);
}

TEST_F(OrderBookTest, DrainingBookResetsBestsAndSize) {
    const auto ids = populate();
    for (const OrderId id : ids) {
        EXPECT_TRUE(book.cancel(id));
    }
    EXPECT_DOUBLE_EQ(book.best_bid(), 0.0);
    EXPECT_DOUBLE_EQ(book.best_ask(), 0.0);
    EXPECT_EQ(book.size(), 0u);
}

}
