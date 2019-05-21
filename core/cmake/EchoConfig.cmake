include(CMakeFindDependencyMacro)
find_dependency(Threads)
find_dependency(clog)
find_dependency(comm)
find_dependency(EventManager)
find_dependency(crossguid)

include(${CMAKE_CURRENT_LIST_DIR}/echoTargets.cmake)
