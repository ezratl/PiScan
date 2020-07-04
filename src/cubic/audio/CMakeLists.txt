target_sources(cubic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioSinkThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioSinkFileThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioFileWAV.cpp
)

target_include_directories(cubic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(piScan_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
