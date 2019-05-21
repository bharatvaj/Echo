cmake_minimum_required(VERSION 3.8)
project(Echo VERSION 0.2.0 LANGUAGES CXX DESCRIPTION "Chat for gamers")

find_package(Threads)
find_package(clog)
find_package(comm)
find_package(EventManager)
find_package(crossguid)
file(GLOB_RECURSE Echo_SRCS "src/*.cpp")

add_library(Echo ${Echo_SRCS})
set_target_properties(Echo PROPERTIES VERSION ${PROJECT_VERSION})

target_compile_features(Echo PUBLIC cxx_enum_forward_declarations)

target_include_directories(Echo PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(Echo PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Echo PRIVATE clog::clog)
target_link_libraries(Echo PRIVATE crossguid)
target_link_libraries(Echo PUBLIC comm::comm)
target_link_libraries(Echo PUBLIC EventManager)

add_subdirectory(test)
