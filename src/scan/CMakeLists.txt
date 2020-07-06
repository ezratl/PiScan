add_library(scan
	Entry.cpp
	RadioSystem.cpp
	SystemList.cpp
	)
	
add_executable(piscan_hpdconv SentinelFile.cpp)
target_link_libraries(piscan_hpdconv scan common boost_system loguru pthread stdc++fs dl)
