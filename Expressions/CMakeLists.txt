cmake_minimum_required(VERSION 3.8)
project(Expressions)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp src/Expression.h src/Number.h src/Variable.h src/BinaryExpression.h src/Sum.h src/Sub.h src/Mul.h src/Div.h src/SinCos.h)
add_executable(Expressions ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")