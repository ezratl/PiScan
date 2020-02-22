project(piScan_backend_tests)

add_executable(piScan_backend_tests EXCLUDE_FROM_ALL test_ScannerStateMachine.cpp)
add_executable(testclient EXCLUDE_FROM_ALL testclient.cpp)
add_executable(test_utils EXCLUDE_FROM_ALL test_utils.cpp)

option(DETACH_KERNEL_DRIVER "Detach kernel driver if loaded" OFF)
if (DETACH_KERNEL_DRIVER)
    message (STATUS "Building with kernel driver detaching enabled")
    add_definitions(-DDETACH_KERNEL_DRIVER=1)
else (DETACH_KERNEL_DRIVER)
    message (STATUS "Building with kernel driver detaching disabled, use -DDETACH_KERNEL_DRIVER=ON to enable")
endif (DETACH_KERNEL_DRIVER)

target_link_libraries(testclient server external pthread boost_system protobuf dl)
target_link_libraries(test_utils pthread)
