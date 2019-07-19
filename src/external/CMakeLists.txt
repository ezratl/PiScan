add_library(external
	atomicops.h
	concurrentqueue.h
	liquid.h
	readerwriterqueue.h
	)
	
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/loguru)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rtaudio)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tinyxml)
	
	target_include_directories(external PUBLIC PiScan-protobuf loguru rtaudio tinyxml)
	target_include_directories(piScan_backend PUBLIC PiScan-protobuf loguru rtaudio tinyxml)
	
		target_sources(external PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/context.pb.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/context.pb.h
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/messages.pb.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/messages.pb.h
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/request.pb.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PiScan-protobuf/request.pb.h
	${CMAKE_CURRENT_SOURCE_DIR}/loguru/loguru.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/loguru/loguru.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/tinystr.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/tinystr.h
	${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/tinyxml.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/tinyxml.h
	${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/tinyxmlerror.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/tinyxmlparser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/rtaudio/RtAudio.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/rtaudio/RtAudio.h
	)
	

