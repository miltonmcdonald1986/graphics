find_package(GTest CONFIG REQUIRED)

if(graphics_ENABLE_TESTING)

    add_executable(graphics_core_tests
        test_logging.cpp
    )

    target_link_libraries(graphics_core_tests
        PRIVATE
            graphics
            GTest::gtest
            GTest::gtest_main
    )

    target_include_directories(graphics_core_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src
    )

    add_test(NAME graphics_core_tests COMMAND graphics_core_tests)

endif()