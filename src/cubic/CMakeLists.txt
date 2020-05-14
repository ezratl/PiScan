add_library(cubic
    CubicSDR.cpp
    AppConfig.cpp
    IOThread.cpp
    SessionMgr.cpp
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
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/analog)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/modem)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/audio)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/util)
	
	target_include_directories(cubic PUBLIC sdr demod modules modules/modem modules/modem/analog audio util)
	target_include_directories(piScan_backend PUBLIC sdr demod modules modules/modem modules/modem/analog audio util)

target_sources(cubic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/sdr/SDRDeviceInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdr/SDRPostThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdr/SDREnumerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdr/SoapySDRThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demod/DemodulatorPreThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demod/DemodulatorThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demod/DemodulatorWorkerThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demod/DemodulatorInstance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demod/DemodulatorMgr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/Modem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/ModemAnalog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/ModemDigital.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/analog/ModemAM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/analog/ModemDSB.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/analog/ModemFM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/analog/ModemNBFM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/analog/ModemFMStereo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/analog/ModemIQ.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/analog/ModemLSB.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/modem/analog/ModemUSB.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/AudioThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/AudioSinkThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/AudioSinkFileThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/AudioFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/AudioFileWAV.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/Timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/DataTree.cpp
)
