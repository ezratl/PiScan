cmake_minimum_required (VERSION 3.13)

cmake_policy(SET CMP0079 NEW)

project (piScan_backend)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set(CMAKE_BUILD_TYPE Debug)

add_executable(piScan_backend piScan_backend.cpp)

execute_process(
	COMMAND protoc -I=${CMAKE_SOURCE_DIR}/external/PiScan-protobuf --cpp_out=${CMAKE_SOURCE_DIR}/external/PiScan-protobuf context.proto
	COMMAND protoc -I=${CMAKE_SOURCE_DIR}/external/PiScan-protobuf --cpp_out=${CMAKE_SOURCE_DIR}/external/PiScan-protobuf messages.proto
	COMMAND protoc -I=${CMAKE_SOURCE_DIR}/external/PiScan-protobuf --cpp_out=${CMAKE_SOURCE_DIR}/external/PiScan-protobuf request.proto
)

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

#no md5 checking for now, need to figure out how to make it work with the text file
#file(DOWNLOAD http://www.live555.com/liveMedia/public/live555-latest-md5.txt ${PROJECT_SOURCE_DIR}/live.md5)
#file(READ ${PROJECT_SOURCE_DIR}/live.md5 LIVE_MD5 HEX)

include(ExternalProject) # Include definition of 'ExternalProject_Add' function

# when using hgfs in a VM guest tar extraction fails, so it can only be extracted on the host
IF (HGFS)
ExternalProject_Add(live555 # Name of the target. Could be any
    # Setup source directory
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/live
    # Setup build directory. Here it is the same as source one.
    BUILD_IN_SOURCE 1
    # Configuration step
    CONFIGURE_COMMAND ./genMakefiles linux
    # Build step. It is actually `make`, but in a wise manner.
    BUILD_COMMAND "make"
    # Disable install step
    INSTALL_COMMAND ""
    # needed for ninja support
    BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/external/live/liveMedia/libliveMedia.a
    	${PROJECT_SOURCE_DIR}/external/live/BasicUsageEnvironment/libBasicUsageEnvironment.a
    	${PROJECT_SOURCE_DIR}/external/live/groupsock/libgroupsock.a
    	${PROJECT_SOURCE_DIR}/external/live/UsageEnvironment/libUsageEnvironment.a
    )
ELSE (HGFS)
ExternalProject_Add(live555 # Name of the target. Could be any
    # Setup source directory
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/live
	
	URL http://www.live555.com/liveMedia/public/live555-latest.tar.gz
	#URL_MD5 ${LIVE_MD5}
    # Setup build directory. Here it is the same as source one.
    BUILD_IN_SOURCE 1
    # Configuration step
    CONFIGURE_COMMAND ./genMakefiles linux
    # Build step. It is actually `make`, but in a wise manner.
    BUILD_COMMAND "make"
    # Disable install step
    INSTALL_COMMAND ""
    # needed for ninja support
    BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/external/live/liveMedia/libliveMedia.a
    	${PROJECT_SOURCE_DIR}/external/live/BasicUsageEnvironment/libBasicUsageEnvironment.a
    	${PROJECT_SOURCE_DIR}/external/live/groupsock/libgroupsock.a
    	${PROJECT_SOURCE_DIR}/external/live/UsageEnvironment/libUsageEnvironment.a
    )
ENDIF (HGFS)
    
add_library(livemedia STATIC IMPORTED GLOBAL)
set_target_properties(livemedia PROPERTIES
	IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/external/live/liveMedia/libliveMedia.a
	)
	
add_library(liveBUE STATIC IMPORTED GLOBAL)
set_target_properties(liveBUE PROPERTIES
	IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/external/live/BasicUsageEnvironment/libBasicUsageEnvironment.a
	)
	
add_library(liveUE STATIC IMPORTED GLOBAL)
set_target_properties(liveUE PROPERTIES
	IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/external/live/UsageEnvironment/libUsageEnvironment.a
	)
	
add_library(liveGS STATIC IMPORTED GLOBAL)
set_target_properties(liveGS PROPERTIES
	IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/external/live/groupsock/libgroupsock.a
	)


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

IF(LIQUID_API_OLD)
	ADD_DEFINITIONS(-DLIQUID_API_OLD)
ENDIF(LIQUID_API_OLD)

# TODO refactoring needed to eliminate this dependency
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cubic/util)
target_include_directories(scan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cubic/util)
# TODO refactoring needed to eliminate this dependency
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cubic/sdr)
target_include_directories(scan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cubic/sdr)

add_dependencies(server proto)

set(LIVE_LIBS livemedia liveBUE liveUE liveGS ssl crypto)

#set(CUBIC_LIBS cubic cubic_demod cubic_modem cubic_sdr cubic_util cubic_audio)

set(SYSTEM_LIBS pthread atomic boost_system protobuf liquid)

set(APPLICATION_LIBS ${SYSTEM_LIBS} ${LIVE_LIBS} ${CUBIC_LIBS} ${OTHER_LIBRARIES})

set(APPLICATION_LIBS core external loguru proto scan server sigproc ${APPLICATION_LIBS})

target_link_libraries(piScan_backend ${APPLICATION_LIBS})

