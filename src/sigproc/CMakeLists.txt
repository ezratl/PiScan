add_library(sigproc
	${CMAKE_CURRENT_SOURCE_DIR}/Demodulator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TunerManager.cpp
)

add_subdirectory(test)
