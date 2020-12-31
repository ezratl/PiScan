add_library(server
	${CMAKE_CURRENT_SOURCE_DIR}/AudioStreamServer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BackendServer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DebugServer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SocketServer.cpp
)

add_subdirectory(connection)
add_subdirectory(audio_server)
add_subdirectory(messages)
add_subdirectory(test)
