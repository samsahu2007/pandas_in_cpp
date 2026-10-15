cmake_minimum_required(VERSION 3.23)
project(my_pandas)

add_library(app)

target_sources(app
    PRIVATE
        DataStruct_class.cpp
)
option(BUILD_TESTING "Enable testing and build tests" OFF)

if()
    # These tests can use the Catch2-provided main
    find_package(Catch2 3 REQUIRED)

    add_executable(tests)
    target_sources(tests 
        PRIVATE
            app
            Tests.cpp
    )

    target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

    enable_testing()
    add_test(NAME RunUnitTests COMMAND unit_tests)
endif()

# These tests need their own main
# add_executable(custom-main-tests test.cpp test-main.cpp)
# target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)