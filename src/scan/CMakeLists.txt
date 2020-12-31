add_library(scan
	${CMAKE_CURRENT_SOURCE_DIR}/Entry.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RadioSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SystemList.cpp
)
	
add_executable(piscan_hpdconv ${CMAKE_CURRENT_SOURCE_DIR}/SentinelFile.cpp)
target_link_libraries(piscan_hpdconv scan common boost_system loguru pthread stdc++fs dl)

add_subdirectory(test)
