target_sources(cubic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/SDRDeviceInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SDRPostThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SDREnumerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SoapySDRThread.cpp
)

target_include_directories(cubic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(piScan_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
