cmake_minimum_required(VERSION 3.10)
project(CAN_FD_Library)

set(CMAKE_CXX_STANDARD 11)

find_package(yaml-cpp REQUIRED)

include_directories(include)

add_library(canfd_lib src/canfd.cpp)

add_executable(can_fd_reader src/socketcanfd_read.cpp)
target_link_libraries(can_fd_reader PRIVATE canfd_lib yaml-cpp)

add_executable(can_fd_writer src/socketcanfd_write.cpp)
target_link_libraries(can_fd_writer PRIVATE canfd_lib yaml-cpp)
