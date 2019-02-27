cmake_minimum_required (VERSION 2.6)

project (piScan_backend)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_executable(piScan_backend piScan_backend.cpp)
target_link_libraries(piScan_backend pthread dl)

include_directories(core drivers external scan server sigproc)

add_subdirectory(core)
add_subdirectory(drivers)
add_subdirectory(external)
add_subdirectory(scan)
add_subdirectory(server)
add_subdirectory(sigproc)

target_link_libraries(piScan_backend core drivers external scan server sigproc)
