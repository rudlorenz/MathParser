cmake_minimum_required(VERSION 3.13)
project(Expressions)

set(CMAKE_CXX_STANDARD 20)

add_library(expressions)

include(FetchContent)
FetchContent_Declare(
        fmtlib
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG origin/master
        GIT_PROGRESS TRUE
        USES_TERMINAL_DOWNLOAD TRUE
)

FetchContent_MakeAvailable(fmtlib)

target_sources(
        expressions
        PRIVATE
            Parser.cpp ParserInternal.h ParserInternal.cpp Stack.h Number.h Variable.h BinaryExpression.h Sum.h Sub.h
            Mul.h Div.h SinCos.h Negate.h
        PUBLIC
            Expression.h Parser.h Expressions.h
)

target_include_directories(
        expressions
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(expressions PUBLIC fmt::fmt)

target_compile_features(expressions PUBLIC cxx_std_20)
target_compile_options(expressions PRIVATE
        $<$<CONFIG:Debug>:
            $<$<CXX_COMPILER_ID:Clang>: -Wall -Wextra -Wshadow -Wpedantic -fno-omit-frame-pointer -g -fno-limit-debug-info
                -Wnon-virtual-dtor -Woverloaded-virtual -Wnull-dereference -Wdouble-promotion -Weffc++>
            $<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra -pedantic -Wshadow -Weffc++ -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -g>>)

target_link_options(expressions PRIVATE
        $<$<CONFIG:Debug>: -fno-omit-frame-pointer>)
