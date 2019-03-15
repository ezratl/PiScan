project(piScan_backend_tests)

add_executable(piScan_backend_tests test_ScannerStateMachine.cpp)
add_executable(test_RtlFm test_RtlFm.cpp)

option(DETACH_KERNEL_DRIVER "Detach kernel driver if loaded" OFF)
if (DETACH_KERNEL_DRIVER)
    message (STATUS "Building with kernel driver detaching enabled")
    add_definitions(-DDETACH_KERNEL_DRIVER=1)
else (DETACH_KERNEL_DRIVER)
    message (STATUS "Building with kernel driver detaching disabled, use -DDETACH_KERNEL_DRIVER=ON to enable")
endif (DETACH_KERNEL_DRIVER)

target_link_libraries(test_RtlFm drivers external pthread rtlsdr)
