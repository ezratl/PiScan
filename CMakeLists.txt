cmake_minimum_required (VERSION 3.12.4)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Wextra")
set(CMAKE_BUILD_TYPE Debug)

project(PiScan)

set(PISCAN_BIN_DIR ${PROJECT_SOURCE_DIR}/build)
file(MAKE_DIRECTORY ${PISCAN_BIN_DIR})

set(LIQUID_API_OLD ON CACHE BOOL "Build with libliquid version <1.3.2")

## GoogleTest configuration
enable_testing()
include(GoogleTest)
add_subdirectory("extern/googletest")

mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

macro(package_add_test TESTNAME)
    # create an exectuable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TESTNAME} gtest gmock gtest_main)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER test)
endmacro()

## ---- ##

add_subdirectory(src)
add_subdirectory(proto)

#target_compile_definitions(common PUBLIC -DDATABASE_PATH="${PROJECT_SOURCE_DIR}/data")

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


## Versioning
execute_process(COMMAND git log --pretty=format:'%h' -n 1
                OUTPUT_VARIABLE GIT_REV
                ERROR_QUIET)

# Check whether we got any revision (which isn't
# always the case, e.g. when someone downloaded a zip
# file from Github instead of a checkout)
if ("${GIT_REV}" STREQUAL "")
    set(GIT_REV "N/A")
    set(GIT_DIFF "")
    set(GIT_TAG "N/A")
    set(GIT_BRANCH "N/A")
else()
    execute_process(
        COMMAND bash -c "git diff --quiet --exit-code || echo -dirty"
        OUTPUT_VARIABLE GIT_DIFF)
    execute_process(
        COMMAND git describe --exact-match --tags
        OUTPUT_VARIABLE GIT_TAG ERROR_QUIET)
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        OUTPUT_VARIABLE GIT_BRANCH)

    string(STRIP "${GIT_REV}" GIT_REV)
    string(SUBSTRING "${GIT_REV}" 1 7 GIT_REV)
    string(STRIP "${GIT_DIFF}" GIT_DIFF)
    string(STRIP "${GIT_TAG}" GIT_TAG)
    string(STRIP "${GIT_BRANCH}" GIT_BRANCH)
endif()

if("${GIT_TAG}" STREQUAL "")
    set(VERSION ${GIT_BRANCH}-${GIT_REV}${GIT_DIFF})
    set(CPACK_PACKAGE_VERSION 0.0.1)
else()
    set(VERSION ${GIT_TAG}${GIT_DIFF})

    if("${GIT_DIFF}" STREQUAL "")
        set(CPACK_PACKAGE_VERSION ${VERSION})
    else()
        set(CPACK_PACKAGE_VERSION 0.0.1)
    endif()
endif()

message("PiScan version: ${VERSION}")
message("Package version: ${CPACK_PACKAGE_VERSION}")
set(VERSION_DEF "#define PISCAN_VERSION \"${VERSION}\"\n")
file(WRITE ${PROJECT_SOURCE_DIR}/src/version.h "${VERSION_DEF}")

set(CPACK_SET_DESTDIR ON)
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "etaimutyloomis@gmail.com")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)


set(DEFAULT_DATA_DIR_LOCAL ON CACHE BOOL "Use the repo as the default data directory rather than /etc/piscan")

if(DEFAULT_DATA_DIR_LOCAL)
    set(DATABASE_PATH "\"${PROJECT_SOURCE_DIR}/data\"")
else(DEFAULT_DATA_DIR_LOCAL)
    ## TODO additional work needed to interact properly with linux FS
    set(DATABASE_PATH "\"/etc/piscan\"")

    install(
        CODE "execute_process(COMMAND sh ${CMAKE_SOURCE_DIR}/scripts/install_defaults.sh)"
    )
endif(DEFAULT_DATA_DIR_LOCAL)

message("PiScan data path: ${DATABASE_PATH}")
target_compile_definitions(common PUBLIC -DDATABASE_PATH=${DATABASE_PATH})
