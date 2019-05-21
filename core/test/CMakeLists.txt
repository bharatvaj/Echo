cmake_minimum_required(VERSION 3.8)

project(echo_test)
add_executable(echo_test echo_test.cpp)

target_link_libraries(echo_test Echo)

add_test(echo_test echo_test)
