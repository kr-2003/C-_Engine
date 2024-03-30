cmake_minimum_required(VERSION 3.0)
project(MyProject)

# Add the include directory to the search path
include_directories(include)

# Add your source files
add_executable(MyProject src/itch_handler/itch_handler.cpp performance/itch_handler_performance.cpp)