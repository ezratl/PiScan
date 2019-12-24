add_library(scan
	Entry.cpp
	Entry.h
	RadioSystem.cpp
	RadioSystem.h
	SystemList.cpp
	SystemList.h
	)
	
add_executable(piscan_hpdconv SentinelFile.cpp)
target_link_libraries(piscan_hpdconv scan core boost_system loguru pthread stdc++fs dl)
