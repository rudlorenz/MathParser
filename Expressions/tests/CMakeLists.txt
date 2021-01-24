cmake_minimum_required(VERSION 3.11)
project(Expressions_tests)

set(CMAKE_CXX_STANDARD 20)


include(CTest)
include(FetchContent)
include(GoogleTest)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        origin/master
)

FetchContent_MakeAvailable(googletest)


add_executable(expression_tests basic_tests.cpp LexerTests.cpp ReverseNotationTests.cpp)

target_link_libraries(expression_tests gtest gtest_main gmock)
target_link_libraries(expression_tests expressions)

gtest_discover_tests(expression_tests)