# Math parser

Simple math symbolic manipulation library.

## Requirements

1. C++17 compatible compiler
2. CMake 3.13

## Usage

Clone

```bash
https://github.com/rudlorenz/mathParser
```

To use Expressions in your project add src folder with `add_subdirectory()` command to your `CmakeLists.txt` file and
include `Expression.h` and `Parser.h` in source files.
Cmake will automatically fetch gtest and fmt library. 

Alternatively you can use supplied `main.cpp`.

## Examples

Parsing expression from string:

```c++
#include <Parser.h>
#include <fmt/core.h>

std::optional<mexpr::Expression> result = parser::parse("x + y");
if (!result.has_value()) {
    fmt::print(stderr, "Something wrong!");
    return 0;
}

fmt::print(stdout, "input : {}\n", result->to_string());
fmt::print(stdout, "as expressions : {}\n", result->to_expr_string());
```

Parse expression from string and find derivative:

```c++
#include <Parser.h>

std::optional<mexpr::Expression> result = parser::parse("x * x + 2*x + 10");
if (!result.has_value()) {
    fmt::print(stderr, "Something wrong!");
    return 0;
}

fmt::print(stdout, "input : {}\n", input);
fmt::print(stdout, "as expressions : {}\n", result->to_expr_string());
fmt::print(stdout, "d/dx : {}\n", result->diff("x").to_string());
```

Calls are chainable:

```c++
auto result = parser::parse("x*x*y + x*y*y");
auto as_string = result
    ? result->diff(x).diff(y).to_string();
    : "empty"; 
```

You can create expressions directly, but it's highly discouraged:

```c++
#include "Expressions.h"
using namespace mexpr;

// x * x + y
auto var_x = Expression{"x"};
auto var_y = Expression{"y"};
auto two_expr = Expression{2};
auto var_x_squared = Expression{
    binary_expr_type::mul,
    var_x.clone(),
    var_x.clone(),
}
auto result = Expression{
    binary_expr_type::sub,
    Expression{
        binary_expr_type::sum,
        std::move(var_x_squared),
        std::move(var_y)
    },
    std::move(two_expr)
}
```

## TODO

1. Unit testing.
2. ~~Proper lexical and syntax analysis i.e. input expression validation. Right now it works "garbage in --> garbage out".~~
3. Expression simplification.
4. ~~Proper parser errors.~~
5. More unit tests.
6. POW operation.
7. Check for proper "unary" function parsing.
8. Expression evaluation.