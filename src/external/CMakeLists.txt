add_library(external
	atomicops.h
	concurrentqueue.h
	loguru.cpp
	loguru.hpp
	readerwriterqueue.h
	convenience.h
	convenience.c
	)
	
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/cpp)
	target_include_directories(external PUBLIC PiScan-protobuf/cpp)
	target_include_directories(piScan_backend PUBLIC PiScan-protobuf/cpp)
	
		target_sources(external PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/cpp/context.pb.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/cpp/context.pb.h
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/cpp/messages.pb.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/cpp/messages.pb.h
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/cpp/request.pb.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/cpp/request.pb.h
	)
	

