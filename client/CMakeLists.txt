cmake_minimum_required(VERSION 3.0)
project(EchoClient VERSION 0.2.0 LANGUAGES CXX DESCRIPTION "Chat for gamers")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../core ${CMAKE_CURRENT_SOURCE_DIR}/../core/build)

find_package(Threads)
find_package(clog)
find_package(comm)
find_package(EventManager)
find_package(fmt)
find_package(shell4cpp)
file(GLOB_RECURSE EchoClientLib_SRCS "src/*.cpp")

add_library(EchoClientLib ${EchoClientLib_SRCS})

set_target_properties(EchoClientLib PROPERTIES VERSION ${PROJECT_VERSION})

target_compile_features(EchoClientLib PRIVATE cxx_enum_forward_declarations)

target_link_libraries(EchoClientLib PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(EchoClientLib PRIVATE clog::clog)
target_link_libraries(EchoClientLib PRIVATE comm::comm)
target_link_libraries(EchoClientLib PRIVATE crosssocket::crosssocket)

target_link_libraries(EchoClientLib PUBLIC Echo)

target_include_directories(EchoClientLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# tests
add_subdirectory(test)

add_executable(EchoClient "echo_client.cpp")
target_link_libraries(EchoClient PRIVATE EchoClientLib)
target_link_libraries(EchoClient PRIVATE fmt::fmt)
target_link_libraries(EchoClient PRIVATE shell4cpp)
