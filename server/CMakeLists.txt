cmake_minimum_required(VERSION 3.0)
project(EchoServer VERSION 0.2.0 LANGUAGES CXX DESCRIPTION "Chat for gamers")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../core ${CMAKE_CURRENT_SOURCE_DIR}/../core/build)

find_package(Threads)
find_package(clog)
find_package(comm)
find_package(EventManager)
find_package(fmt)
find_package(shell4cpp)
find_package(SQLite::SQLite3)

file(GLOB_RECURSE EchoServer_SRCS "src/*.cpp")
add_executable(EchoServer ${EchoServer_SRCS} "echo_server.cpp")
set_target_properties(EchoServer PROPERTIES VERSION ${PROJECT_VERSION})
target_compile_features(EchoServer PRIVATE cxx_enum_forward_declarations)

target_link_libraries(EchoServer PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(EchoServer PRIVATE clog)
target_link_libraries(EchoServer PRIVATE crosssocket)
target_link_libraries(EchoServer PRIVATE comm::comm)
target_link_libraries(EchoServer PRIVATE fmt::fmt)
target_link_libraries(EchoServer PRIVATE shell4cpp)
target_link_libraries(EchoServer PRIVATE Echo)
target_link_libraries(EchoServer PRIVATE SQLite::SQLite3)

target_include_directories(EchoServer PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# tests
add_subdirectory(test)
