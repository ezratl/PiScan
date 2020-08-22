add_library(server
	AudioStreamServer.cpp
	BackendServer.cpp
	DebugServer.cpp
	SocketServer.cpp
)

add_subdirectory(connection)
add_subdirectory(audio_server)
add_subdirectory(messages)
add_subdirectory(test)
