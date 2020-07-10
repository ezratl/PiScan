target_sources(external PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/RtAudio.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RtAudio.h
)

target_include_directories(external PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(piscan_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
