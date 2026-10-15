cmake_minimum_required(VERSION 3.23)
project(my_pandas)

add_library(app)

target_sources(app
    PRIVATE
        DataStruct_class.cpp
)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

option(BUILD_TESTING "Enable testing and build tests" OFF)

if(BUILD_TESTING)
    # These tests can use the Catch2-provided main
    find_package(Catch2 3 REQUIRED)

    add_executable(unit_tests)
    target_sources(unit_tests 
        PRIVATE
            Tests.cpp
    )

    target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain app)

    enable_testing()
    add_test(NAME RunUnitTests COMMAND unit_tests)

    add_custom_command(
        TARGET unit_tests 
        POST_BUILD
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R RunUnitTests
        COMMENT "Automatically running Catch2 unit tests..."
    )
endif()

# These tests need their own main
# add_executable(custom-main-tests test.cpp test-main.cpp)
# target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)