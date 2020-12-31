add_library(external
	${CMAKE_CURRENT_SOURCE_DIR}/atomicops.h
	${CMAKE_CURRENT_SOURCE_DIR}/concurrentqueue.h
	${CMAKE_CURRENT_SOURCE_DIR}/readerwriterqueue.h
)
	
add_library(loguru)
#add_library(proto)
	
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/loguru)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rtaudio)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tinyxml)
	
#target_include_directories(loguru PUBLIC loguru)
#target_include_directories(external PUBLIC loguru rtaudio)
#target_include_directories(proto PUBLIC PiScan-protobuf)
#target_include_directories(piscan_server PUBLIC PiScan-protobuf loguru rtaudio)
target_include_directories(piscan_server PUBLIC PiScan-protobuf)
	
#set_source_files_properties(
#	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/context.pb.cc PROPERTIES GENERATED 1
#	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/context.pb.h PROPERTIES GENERATED 1
#	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/messages.pb.cc PROPERTIES GENERATED 1
#	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/messages.pb.h PROPERTIES GENERATED 1
#	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/request.pb.cc PROPERTIES GENERATED 1
#	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/request.pb.h PROPERTIES GENERATED 1
#)
