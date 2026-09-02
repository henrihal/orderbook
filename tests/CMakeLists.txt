find_package(GTest CONFIG REQUIRED)

add_executable(orders_test orders_test.cpp)
target_link_libraries(orders_test PRIVATE orderbook_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(orders_test)
