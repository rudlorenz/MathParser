cmake_minimum_required(VERSION 3.13)
project(Expressions)

set(CMAKE_CXX_STANDARD 20)

add_library(expressions)

target_sources(
        expressions
        PRIVATE
            Parser.cpp
        PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/Expression.h ${CMAKE_CURRENT_LIST_DIR}/Number.h
            ${CMAKE_CURRENT_LIST_DIR}/Variable.h ${CMAKE_CURRENT_LIST_DIR}/BinaryExpression.h
            ${CMAKE_CURRENT_LIST_DIR}/Sum.h ${CMAKE_CURRENT_LIST_DIR}/Sub.h ${CMAKE_CURRENT_LIST_DIR}/Mul.h
            ${CMAKE_CURRENT_LIST_DIR}/Div.h ${CMAKE_CURRENT_LIST_DIR}/SinCos.h ${CMAKE_CURRENT_LIST_DIR}/Parser.h
            ${CMAKE_CURRENT_LIST_DIR}/Negate.h
)

target_include_directories(
        expressions
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)


target_compile_features(expressions PUBLIC cxx_std_20)
target_compile_options(expressions PRIVATE
        $<$<CONFIG:Debug>:
            $<$<CXX_COMPILER_ID:Clang>: -Wall -Wextra -Wshadow -Wpedantic -fno-omit-frame-pointer -g -fno-limit-debug-info
                -Wnon-virtual-dtor -Woverloaded-virtual -Wnull-dereference -Wdouble-promotion -Weffc++ -fsanitize=address>
            $<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra -pedantic -Wshadow -Weffc++ -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -g>>)

target_link_options(expressions PRIVATE
        $<$<CONFIG:Debug>: -fno-omit-frame-pointer -fsanitize=address>)
