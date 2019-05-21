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

set(ECHO_INC_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(ECHO_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}")
set(ECHO_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
set(ECHO_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
set(ECHO_FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
set(ECHO_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/Echo/cmake")
set(ECHO_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/Echo")

# Install target
install(TARGETS Echo EXPORT EchoTargets
  RUNTIME       DESTINATION ${ECHO_RUNTIME_INSTALL_DIR}
  LIBRARY       DESTINATION ${ECHO_LIBRARY_INSTALL_DIR}
  ARCHIVE       DESTINATION ${ECHO_ARCHIVE_INSTALL_DIR}
  FRAMEWORK     DESTINATION ${ECHO_FRAMEWORK_INSTALL_DIR}
)

# Install headers
# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
#   DESTINATION ${ECHO_INC_INSTALL_DIR}
#)

# Install targets
install(EXPORT EchoTargets
	FILE EchoTargets.cmake
  NAMESPACE Echo::
  DESTINATION ${ECHO_CMAKE_CONFIG_INSTALL_DIR}
)

# Install configs
install(FILES cmake/EchoConfig.cmake
  DESTINATION ${ECHO_CMAKE_CONFIG_INSTALL_DIR}
)

# Install readme and license
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
  DESTINATION ${ECHO_ADDITIONAL_FILES_INSTALL_DIR}
)

add_subdirectory(test)
