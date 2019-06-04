add_library(external
	atomicops.h
	concurrentqueue.h
	loguru.cpp
	loguru.hpp
	readerwriterqueue.h
	convenience.h
	convenience.c
	)
	
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf)
	target_include_directories(external PUBLIC PiScan-protobuf)
	target_include_directories(piScan_backend PUBLIC PiScan-protobuf)
	
		target_sources(external PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/context.pb.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/context.pb.h
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/messages.pb.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/messages.pb.h
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/request.pb.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/request.pb.h
	)
	

