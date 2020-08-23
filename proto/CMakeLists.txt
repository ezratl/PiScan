INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PROTO_FILES 
	messages.proto 
	request.proto 
	context.proto
)

PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${PROTO_FILES})
protobuf_generate_python(PROTO_PY ${PROTO_FILES})
add_custom_target(proto_py ALL DEPENDS ${PROTO_PY})

ADD_LIBRARY(proto ${PROTO_HEADER} ${PROTO_SRC} ${PROTO_PY})

#target_sources(proto PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}/context.pb.cc
#	${CMAKE_CURRENT_SOURCE_DIR}/context.pb.h
#	${CMAKE_CURRENT_SOURCE_DIR}/messages.pb.cc
#	${CMAKE_CURRENT_SOURCE_DIR}/messages.pb.h
#	${CMAKE_CURRENT_SOURCE_DIR}/request.pb.cc
#	${CMAKE_CURRENT_SOURCE_DIR}/request.pb.h
#)

#target_include_directories(proto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
