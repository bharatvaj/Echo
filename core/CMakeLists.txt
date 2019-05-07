cmake_minimum_required(VERSION 3.8)
project(echo VERSION 0.0.1 LANGUAGES CXX DESCRIPTION "Chat for gamers")
find_package(Threads)
find_package(clog)
find_package(comm)
find_package(EventManager)
find_package(crossguid)
file(GLOB_RECURSE echo_HDRS "include/*.hpp")
file(GLOB_RECURSE echo_SRCS "src/*.cpp")

add_library(echo ${echo_HDRS} ${echo_SRCS})

set_target_properties(echo PROPERTIES VERSION ${PROJECT_VERSION})

target_compile_features(echo PRIVATE cxx_enum_forward_declarations)

target_link_libraries(echo ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(echo ${CLOG_LIBS})
target_link_libraries(echo ${COMM_LIBS})
target_link_libraries(echo crossguid)

target_include_directories(echo PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_include_directories(echo PRIVATE ${CLOG_INCLUDE_DIR})
target_include_directories(echo PRIVATE ${EM_INCLUDE_DIR})
target_include_directories(echo PRIVATE ${COMM_INCLUDE_DIR})
target_include_directories(echo PRIVATE crossguid)

install(FILES echoConfig.cmake DESTINATION lib/cmake/echo)
install(FILES ${echo_HDRS} DESTINATION include/echo)
install(TARGETS echo
        PUBLIC_HEADER DESTINATION include/echo
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

add_subdirectory(test)
