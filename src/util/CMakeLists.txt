add_library(util
	${CMAKE_CURRENT_SOURCE_DIR}/Thread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StateMachine.cpp
)

add_subdirectory(test)
