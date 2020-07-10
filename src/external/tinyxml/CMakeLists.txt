target_sources(external PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/tinystr.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tinyxml.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxmlerror.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxmlparser.cpp
)

target_include_directories(external PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(piscan_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
