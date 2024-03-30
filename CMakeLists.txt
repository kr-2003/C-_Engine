cmake_minimum_required(VERSION 3.0)
project(TradingEngine)

# Add the include directory to the search path
include_directories(include)

# Add your source files
add_executable(itch_handler_performance src/itch_handler/itch_handler.cpp performance/itch_handler_performance.cpp)