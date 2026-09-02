cmake_minimum_required(VERSION 3.16)

project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(orderbook_lib orders.cpp)
target_include_directories(orderbook_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(orderbook main.cpp)
target_link_libraries(orderbook PRIVATE orderbook_lib)

enable_testing()
add_subdirectory(tests)
