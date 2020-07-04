target_sources(cubic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/DemodulatorPreThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DemodulatorThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DemodulatorWorkerThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DemodulatorInstance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DemodulatorMgr.cpp
)

target_include_directories(cubic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(piScan_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
