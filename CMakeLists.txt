cmake_minimum_required (VERSION 3.13)

cmake_policy(SET CMP0079 NEW)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Wextra")
set(CMAKE_BUILD_TYPE Debug)

project(PiScan)

set(PISCAN_BIN_DIR ${PROJECT_SOURCE_DIR}/build)
file(MAKE_DIRECTORY ${PISCAN_BIN_DIR})

add_subdirectory(src)
add_subdirectory(proto)

target_compile_definitions(common PUBLIC -DDATABASE_PATH="${PROJECT_SOURCE_DIR}/data")

## External library: LiveMedia ##

#no md5 checking for now, need to figure out how to make it work with the text file
#file(DOWNLOAD http://www.live555.com/liveMedia/public/live555-latest-md5.txt ${PROJECT_SOURCE_DIR}/live.md5)
#file(READ ${PROJECT_SOURCE_DIR}/live.md5 LIVE_MD5 HEX)

include(ExternalProject) # Include definition of 'ExternalProject_Add' function

# when using hgfs in a VM guest tar extraction fails, so it can only be extracted on the host
IF (HGFS)
ExternalProject_Add(live555 # Name of the target. Could be any
    # Setup source directory
    SOURCE_DIR ${PISCAN_BIN_DIR}/live
    # Setup build directory. Here it is the same as source one.
    BUILD_IN_SOURCE 1
    # Configuration step
    CONFIGURE_COMMAND ./genMakefiles linux
    # Build step. It is actually `make`, but in a wise manner.
    BUILD_COMMAND "make"
    # Disable install step
    INSTALL_COMMAND "make"
    # needed for ninja support
    BUILD_BYPRODUCTS ${PISCAN_BIN_DIR}/live/liveMedia/libliveMedia.a
    	${PISCAN_BIN_DIR}/live/BasicUsageEnvironment/libBasicUsageEnvironment.a
    	${PISCAN_BIN_DIR}/live/groupsock/libgroupsock.a
    	${PISCAN_BIN_DIR}/live/UsageEnvironment/libUsageEnvironment.a
    )
ELSE (HGFS)
ExternalProject_Add(live555 # Name of the target. Could be any
    # Setup source directory
    SOURCE_DIR ${PISCAN_BIN_DIR}/live
	
	URL http://www.live555.com/liveMedia/public/live555-latest.tar.gz
	#URL_MD5 ${LIVE_MD5}
    # Setup build directory. Here it is the same as source one.
    BUILD_IN_SOURCE 1
    # Configuration step
    CONFIGURE_COMMAND ./genMakefiles linux
    # Build step. It is actually `make`, but in a wise manner.
    BUILD_COMMAND "make"
    # Disable install step
    INSTALL_COMMAND "make"
    # needed for ninja support
    BUILD_BYPRODUCTS ${PISCAN_BIN_DIR}/live/liveMedia/libliveMedia.a
    	${PISCAN_BIN_DIR}/live/BasicUsageEnvironment/libBasicUsageEnvironment.a
    	${PISCAN_BIN_DIR}/live/groupsock/libgroupsock.a
    	${PISCAN_BIN_DIR}/live/UsageEnvironment/libUsageEnvironment.a
    )
ENDIF (HGFS)
target_include_directories(server PUBLIC ${PISCAN_BIN_DIR}/live/liveMedia/include)
target_include_directories(server PUBLIC ${PISCAN_BIN_DIR}/live/groupsock/include)
target_include_directories(server PUBLIC ${PISCAN_BIN_DIR}/live/UsageEnvironment/include)
target_include_directories(server PUBLIC ${PISCAN_BIN_DIR}/live/BasicUsageEnvironment/include)


    
add_library(livemedia STATIC IMPORTED GLOBAL)
set_target_properties(livemedia PROPERTIES
	IMPORTED_LOCATION ${PISCAN_BIN_DIR}/live/liveMedia/libliveMedia.a
	)
	
add_library(liveBUE STATIC IMPORTED GLOBAL)
set_target_properties(liveBUE PROPERTIES
	IMPORTED_LOCATION ${PISCAN_BIN_DIR}/live/BasicUsageEnvironment/libBasicUsageEnvironment.a
	)
	
add_library(liveUE STATIC IMPORTED GLOBAL)
set_target_properties(liveUE PROPERTIES
	IMPORTED_LOCATION ${PISCAN_BIN_DIR}/live/UsageEnvironment/libUsageEnvironment.a
	)
	
add_library(liveGS STATIC IMPORTED GLOBAL)
set_target_properties(liveGS PROPERTIES
	IMPORTED_LOCATION ${PISCAN_BIN_DIR}/live/groupsock/libgroupsock.a
	)

set(LIVE_LIBS livemedia liveBUE liveUE liveGS)


target_include_directories(server PUBLIC ${PROJECT_SOURCE_DIR}/proto)
