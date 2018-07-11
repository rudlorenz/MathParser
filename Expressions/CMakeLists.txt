cmake_minimum_required(VERSION 3.8)
project(Expressions)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -pedantic -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion \
 -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC \
  -D_FORTIFY_SOURCE=2 -fstack-protector")

include_directories(src)

set(SOURCE_FILES main.cpp)
set(HEADER_FILES src/Expression.h src/Number.h src/Variable.h src/BinaryExpression.h src/Sum.h src/Sub.h src/Mul.h src/Div.h src/SinCos.h)

add_executable(Expressions ${SOURCE_FILES} ${HEADER_FILES})