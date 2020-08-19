project (piscan_server)

add_executable(piscan_server piScan_backend.cpp)

MACRO(use_included_rtaudio)
    SET (cubicsdr_sources
        ${cubicsdr_sources}
        external/rtaudio/RtAudio.cpp
    )
    SET (cubicsdr_headers
        ${cubicsdr_headers}
        external/rtaudio/RtAudio.h
    )
    SOURCE_GROUP("_ext-RTAudio" REGULAR_EXPRESSION "external/rtaudio/.*${REG_EXT}")
    include_directories(${PROJECT_SOURCE_DIR}/external/rtaudio)
ENDMACRO(use_included_rtaudio)

IF (UNIX AND NOT APPLE)
    set(BUILD_DEB OFF CACHE BOOL "Build DEB")

    SET(USE_SYSTEM_RTAUDIO OFF CACHE BOOL "Use the system RtAudio which in turn provides OSS, ALSA, JACK, PulseAudio support depending on how it was compiled")

    SET(LIQUID_LIB liquid)
    SET(OTHER_LIBRARIES ${OTHER_LIBRARIES} dl stdc++fs)

    IF(USE_SYSTEM_RTAUDIO)
        find_package(RtAudio)
        SET(OTHER_LIBRARIES ${OTHER_LIBRARIES} ${RTAUDIO_LIBRARIES})
        ADD_DEFINITIONS(${RTAUDIO_DEFINITIONS})
        include_directories(${RTAUDIO_INCLUDE_DIR})
    ELSE(USE_SYSTEM_RTAUDIO)
        use_included_rtaudio()

        SET(USE_AUDIO_PULSE ON CACHE BOOL "Use Pulse Audio")
        SET(USE_AUDIO_JACK OFF CACHE BOOL "Use Jack Audio")
        SET(USE_AUDIO_ALSA OFF CACHE BOOL "Use ALSA Audio")
        SET(USE_AUDIO_OSS OFF CACHE BOOL "Use OSS Audio")
  
        IF(USE_AUDIO_PULSE)
           SET (OTHER_LIBRARIES ${OTHER_LIBRARIES} pulse-simple pulse)
           ADD_DEFINITIONS(
                -D__LINUX_PULSE__
           )
        ENDIF(USE_AUDIO_PULSE)

        IF(USE_AUDIO_JACK)
           find_package(Jack)
           SET (OTHER_LIBRARIES ${OTHER_LIBRARIES} ${JACK_LIBRARIES})
           ADD_DEFINITIONS(
                -D__UNIX_JACK__
           )
           include_directories(${JACK_INCLUDE_DIRS})
        ENDIF(USE_AUDIO_JACK)

        IF(USE_AUDIO_ALSA)
           SET (OTHER_LIBRARIES ${OTHER_LIBRARIES} asound)
           set(ALSA_INCLUDE_DIR "/usr/include" CACHE FILEPATH "ALSA include path")
           include_directories(${ALSA_INCLUDE_DIR})
           set(ALSA_LIB_DIR "/usr/lib" CACHE FILEPATH "ALSA lib path")
           link_directories(${ALSA_LIB_DIR})
           ADD_DEFINITIONS(
               -D__LINUX_ALSA__
           )
        ENDIF(USE_AUDIO_ALSA)

        IF(USE_AUDIO_OSS)
           SET (OTHER_LIBRARIES ${OTHER_LIBRARIES} oss)
           ADD_DEFINITIONS(
                -D__LINUX_OSS__
           )
        ENDIF(USE_AUDIO_OSS)
    ENDIF(USE_SYSTEM_RTAUDIO)
ELSE(UNIX AND NOT APPLE)
    use_included_rtaudio()
ENDIF(UNIX AND NOT APPLE)




include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	common 
	cubic 
	external 
	scan 
	server 
	sigproc
	util 
#	external/PiScan-protobuf 
	external/loguru
	external/tinyxml
	external/live/liveMedia/include
	external/live/groupsock/include
	external/live/UsageEnvironment/include
	external/live/BasicUsageEnvironment/include
    # TODO these includes need refactored into a lib interface
	cubic/sdr
	cubic/demod
	cubic/modules
	cubic/modules/modem
	cubic/modules/modem/analog
	cubic/audio
	cubic/util
)

add_subdirectory(common)
add_subdirectory(cubic)
add_subdirectory(external)
add_subdirectory(scan)
add_subdirectory(server)
add_subdirectory(sigproc)
add_subdirectory(tests)
add_subdirectory(util)

find_package(SoapySDR "0.4.0" NO_MODULE REQUIRED)
include_directories(${SOAPY_SDR_INCLUDE_DIR})
SET(OTHER_LIBRARIES ${SOAPY_SDR_LIBRARY} ${OTHER_LIBRARIES})
ADD_DEFINITIONS(
    -DUSE_SOAPY_SDR=1
)   

IF(LIQUID_API_OLD)
	ADD_DEFINITIONS(-DLIQUID_API_OLD)
ENDIF(LIQUID_API_OLD)

# TODO refactoring needed to eliminate this dependency
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cubic/util)
target_include_directories(scan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cubic/util)
# TODO refactoring needed to eliminate this dependency
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cubic/sdr)
target_include_directories(scan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cubic/sdr)

target_sources(piscan_server PUBLIC 
	ScannerSM.cpp
	ServerManager.cpp
)

add_dependencies(server proto)

set(LIVE_LIBS livemedia liveBUE liveUE liveGS ssl crypto)

#set(CUBIC_LIBS cubic cubic_demod cubic_modem cubic_sdr cubic_util cubic_audio)

set(SYSTEM_LIBS pthread atomic boost_system protobuf liquid)

set(APPLICATION_LIBS ${SYSTEM_LIBS} ${LIVE_LIBS} ${CUBIC_LIBS} ${OTHER_LIBRARIES})

set(APPLICATION_LIBS common external loguru proto scan server sigproc util ${APPLICATION_LIBS})

target_link_libraries(piscan_server ${APPLICATION_LIBS})

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "etaimutyloomis@gmail.com")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)

install(
	TARGETS piscan_server
	DESTINATION /usr/local/bin
    LIBRARY DESTINATION /usr/local/bin
)

