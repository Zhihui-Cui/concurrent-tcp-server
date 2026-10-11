cmake_minimum_required(VERSION 3.20)
project(concurrent_tcp_server LANGUAGES CXX)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "Build this project inside Ubuntu/WSL using: bash dev.sh build")
endif()

add_executable(tcp_server src/server.cpp)
target_compile_features(tcp_server PRIVATE cxx_std_17)
set_target_properties(tcp_server PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(tcp_server PRIVATE
    "$<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>"
)
