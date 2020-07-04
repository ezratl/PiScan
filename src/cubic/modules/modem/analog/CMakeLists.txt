target_sources(cubic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ModemAM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModemDSB.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModemFM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModemNBFM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModemFMStereo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModemIQ.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModemLSB.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModemUSB.cpp
)

target_include_directories(cubic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(piScan_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
