cmake_minimum_required(VERSION 3.8)

project(echo_test)
add_executable(echo_test echo_test.cpp)

target_link_libraries(echo_test echo)
target_link_libraries(echo_test crosssocket)
target_link_libraries(echo_test ${COMM_LIBS})


target_include_directories(echo_test PRIVATE echo)
target_include_directories(echo_test PRIVATE crosssocket)
target_include_directories(echo_test PRIVATE ${COMM_INCLUDE_DIR})
add_test(echo_test echo_test)
