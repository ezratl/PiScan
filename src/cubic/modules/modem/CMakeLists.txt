target_sources(cubic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Modem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModemAnalog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModemDigital.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/analog)
target_include_directories(cubic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(piscan_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
