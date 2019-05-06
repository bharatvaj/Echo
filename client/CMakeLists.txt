cmake_minimum_required(VERSION 3.0)
project(echo_client VERSION 0.0.1 LANGUAGES CXX DESCRIPTION "Chat for gamers")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../core ${CMAKE_CURRENT_SOURCE_DIR}/../core/build)

find_package(Threads)
find_package(clog)
find_package(comm)
find_package(EventManager)
find_package(crossguid)
find_package(crosssocket)
file(GLOB_RECURSE echo_client_HDRS "include/*.hpp")
file(GLOB_RECURSE echo_client_SRCS "src/*.cpp" "echo_client.cpp")

add_executable(echo_client ${echo_client_HDRS} ${echo_client_SRCS})

set_target_properties(echo_client PROPERTIES VERSION ${PROJECT_VERSION})

target_compile_features(echo_client PRIVATE cxx_enum_forward_declarations)

target_link_libraries(echo_client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(echo_client ${CLOG_LIBS})
target_link_libraries(echo_client crossguid)
target_link_libraries(echo_client crosssocket)
target_link_libraries(echo_client echo)

target_include_directories(echo_client PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_include_directories(echo_client PRIVATE ${CLOG_INCLUDE_DIR})
target_include_directories(echo_client PRIVATE ${EM_INCLUDE_DIR})
target_include_directories(echo_client PRIVATE crosssocket)
target_include_directories(echo_client PRIVATE crossguid)
target_include_directories(echo_client PRIVATE echo)

install(FILES echo_clientConfig.cmake DESTINATION lib/cmake/echo_client)
install(FILES ${echo_client_HDRS} DESTINATION include/echo_client)
install(TARGETS echo_client
        PUBLIC_HEADER DESTINATION include/echo_client
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

add_subdirectory(test)
