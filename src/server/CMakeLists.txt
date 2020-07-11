add_library(server
	AudioStreamServer.cpp
	BackendServer.cpp
	DebugServer.cpp
	SocketServer.cpp
)

add_subdirectory(connection)
add_subdirectory(audio_server)
add_subdirectory(messages)

#INCLUDE(FindProtobuf)
#FIND_PACKAGE(Protobuf "3.7.0" REQUIRED)
#INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
