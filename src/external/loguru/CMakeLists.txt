target_sources(loguru PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/loguru.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/loguru.hpp
)

target_include_directories(loguru PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(piScan_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
