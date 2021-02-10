package_add_test(common_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/Configuration_test.cpp
)

target_link_libraries(common_tests common loguru stdc++fs dl)
