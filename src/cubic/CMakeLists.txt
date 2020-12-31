add_library(cubic
    ${CMAKE_CURRENT_SOURCE_DIR}/CubicSDR.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AppConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IOThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SessionMgr.cpp
)

#	SET(LIQUID_API_OLD OFF)
	IF(LIQUID_API_OLD)
		ADD_DEFINITIONS(-DLIQUID_API_OLD)
	ENDIF(LIQUID_API_OLD)

SET(CUBICSDR_VERSION_MAJOR "0")
SET(CUBICSDR_VERSION_MINOR "2")
SET(CUBICSDR_VERSION_PATCH "5")
SET(CUBICSDR_VERSION_SUFFIX "")
SET(CUBICSDR_VERSION "${CUBICSDR_VERSION_MAJOR}.${CUBICSDR_VERSION_MINOR}.${CUBICSDR_VERSION_PATCH}${CUBICSDR_VERSION_SUFFIX}")

SET(CPACK_PACKAGE_VERSION "${CUBICSDR_VERSION_MAJOR}.${CUBICSDR_VERSION_MINOR}.${CUBICSDR_VERSION_PATCH}")
SET(CPACK_PACKAGE_VERSION_MAJOR ${CUBICSDR_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${CUBICSDR_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${CUBICSDR_VERSION_PATCH})

	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sdr)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/demod)
	
	
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/modem)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/audio)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/util)
	
#	target_include_directories(cubic PUBLIC sdr demod modules modules/modem modules/modem/analog audio util)
#	target_include_directories(piscan_server PUBLIC sdr demod modules modules/modem modules/modem/analog audio util)
target_include_directories(piscan_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#set(CUBIC_LIBS cubic cubic_demod cubic_modem cubic_sdr cubic_util cubic_audio)

#add_dependencies(cubic cubic_modem cubic_sdr cubic_util cubic_audio)

#target_link_libraries(cubic LINK_PUBLIC cubic_demod cubic_modem cubic_sdr cubic_util cubic_audio)
#target_link_libraries(piscan_server cubic)
