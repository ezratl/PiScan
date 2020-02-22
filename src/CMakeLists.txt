cmake_minimum_required (VERSION 2.6)

project (piScan_backend)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_BUILD_TYPE Debug)

add_executable(piScan_backend piScan_backend.cpp)

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
	core 
	cubic 
	external 
	scan 
	server 
	sigproc
	util 
	external/PiScan-protobuf 
	external/loguru
	external/tinyxml
	cubic/sdr
	cubic/demod
	cubic/modules
	cubic/modules/modem
	cubic/modules/modem/analog
	cubic/audio
	cubic/util
)

add_subdirectory(core)
add_subdirectory(cubic)
add_subdirectory(external)
add_subdirectory(scan)
add_subdirectory(server)
add_subdirectory(sigproc)
add_subdirectory(tests)

find_package(SoapySDR "0.4.0" NO_MODULE REQUIRED)
include_directories(${SOAPY_SDR_INCLUDE_DIR})
SET(OTHER_LIBRARIES ${SOAPY_SDR_LIBRARY} ${OTHER_LIBRARIES})
ADD_DEFINITIONS(
    -DUSE_SOAPY_SDR=1
)    

target_link_libraries(piScan_backend core cubic external loguru scan server sigproc 
	pthread boost_system protobuf liquid ${OTHER_LIBRARIES})


