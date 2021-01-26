cmake_minimum_required(VERSION 3.13)
project(Expressions)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

add_executable(expressions_main main.cpp)
target_link_libraries(expressions_main
        PUBLIC expressions
        PRIVATE fmt::fmt)

target_compile_features(expressions_main PUBLIC cxx_std_20)
target_compile_options(expressions_main PRIVATE
        $<$<CONFIG:Debug>:
        $<$<CXX_COMPILER_ID:Clang>: -Wall -Wextra -Wshadow -Wpedantic -fno-omit-frame-pointer -g -fno-limit-debug-info
        -Wnon-virtual-dtor -Woverloaded-virtual -Wnull-dereference -Wdouble-promotion -Weffc++>
        $<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra -pedantic -Wshadow -Weffc++ -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -g>>)

target_link_options(expressions_main PRIVATE
        $<$<CONFIG:Debug>: -fno-omit-frame-pointer>)