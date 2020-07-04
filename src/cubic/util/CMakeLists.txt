target_sources(cubic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataTree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ThreadBlockingQueue.cpp
)

target_include_directories(cubic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(piScan_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
