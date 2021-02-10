add_library(common
	${CMAKE_CURRENT_SOURCE_DIR}/Configuration.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/State.cpp
)

add_subdirectory(test)
