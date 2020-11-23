cmake_minimum_required(VERSION 3.8)
project(Expressions)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

set(SOURCE_FILES main.cpp src/Parser.cpp)
set(HEADER_FILES src/Expression.h src/Number.h src/Variable.h src/BinaryExpression.h src/Sum.h src/Sub.h src/Mul.h
        src/Div.h src/SinCos.h src/Parser.h)

add_executable(expressions ${SOURCE_FILES} ${HEADER_FILES})

target_compile_options(expressions PRIVATE
        $<$<CONFIG:Debug>:
        $<$<CXX_COMPILER_ID:Clang>: -Wall -Wextra -Wpedantic -fno-omit-frame-pointer -g>
        $<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra -pedantic -Wshadow -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -g>>)

target_link_options(expressions PRIVATE
        $<$<CONFIG:Debug>: -fno-omit-frame-pointer>)
