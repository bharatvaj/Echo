cmake_minimum_required(VERSION 3.0)
project(echo_server VERSION 0.1.0 LANGUAGES CXX DESCRIPTION "Chat for gamers")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../core ${CMAKE_CURRENT_SOURCE_DIR}/../core/build)

find_package(Threads)
find_package(clog)
find_package(comm)
find_package(EventManager)
find_package(crossguid)
find_package(crosssocket)
find_package(fmt)
include(3rdparty/FindSQLite3.cmake)
file(GLOB_RECURSE echo_server_HDRS "include/*.hpp")
file(GLOB_RECURSE echo_server_SRCS "src/*.cpp" "echo_server.cpp")

add_executable(echo_server ${echo_server_HDRS} ${echo_server_SRCS})

set_target_properties(echo_server PROPERTIES VERSION ${PROJECT_VERSION})

target_compile_features(echo_server PRIVATE cxx_enum_forward_declarations)

target_link_libraries(echo_server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(echo_server ${CLOG_LIBS})
target_link_libraries(echo_server crosssocket)
target_link_libraries(echo_server ${COMM_LIBS})
target_link_libraries(echo_server crossguid)
target_link_libraries(echo_server echo)
target_link_libraries(echo_server ${SQLite3_LIBRARY})
target_link_libraries(echo_server fmt)

target_include_directories(echo_server PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_include_directories(echo_server PRIVATE ${CLOG_INCLUDE_DIR})
target_include_directories(echo_server PRIVATE ${EM_INCLUDE_DIR})
target_include_directories(echo_server PRIVATE crosssocket)
target_include_directories(echo_server PRIVATE ${COMM_INCLUDE_DIR})
target_include_directories(echo_server PRIVATE crossguid)
target_include_directories(echo_server PRIVATE echo)
target_include_directories(echo_server PRIVATE ${SQLite3_INCLUDE_DIR})
target_include_directories(echo_server PRIVATE fmt)

install(FILES echo_serverConfig.cmake DESTINATION lib/cmake/echo_server)
install(FILES ${echo_server_HDRS} DESTINATION include/echo_server)
install(TARGETS echo_server
        PUBLIC_HEADER DESTINATION include/echo_server
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

add_subdirectory(test)
