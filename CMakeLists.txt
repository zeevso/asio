cmake_minimum_required(VERSION 3.12)
project(syn_client)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(Boost COMPONENTS system thread REQUIRED)

add_executable(synclient synclient.cpp)
target_link_libraries(synclient ${Boost_LIBRARIES})
