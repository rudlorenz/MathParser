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

To use Expressions in your project add src folder with `add_subdirectory()` command to your `CmakeLists.txt` file 
and include `Expressions.h` and `Parser.h` in source files.

Alternatively you can use supplied `main.cpp`.

## Examples

Parsing expression from string:

```c++
#include <Parser.h>

auto result = parser::parse("x + y");
if (result != nullptr) {
    std::cout << "result : " << result->tostring();
}
```

Parse expression from string and find derivative:

```c++
#include <Parser.h>

auto result = parser::parse("x * x + 2*x + 10");
if (result != nullptr)
{
    auto derivative = result->diff("x");
    std::cout << "expression : " << result->tostring() << "\n"
        << "derivative : " << derivative->tostring(); 
}
```

Calls are chainable:

```c++
auto result = parser::parse("x*x*y + x*y*y");
auto as_string = result != nullptr
    ? result->diff(x)->diff(y)->tostring();
```

You can create expressions directly, but it's highly discouraged:

```c++
#include "Expressions.h"
    // x * x + y
    auto var_x = std::make_shared<Variable>("x");
    auto var_y = std::make_shared<Variable>("y"); 
    auto result = std::make_shared<Sum>(
        std::make_shared<Mul>(var_x, var_x),
        var_y
    );
```

## TODO

1. Unit testing.
2. Proper lexical and synax analysis i.e. input expression validation. Right now it works "garbage in --> garbage out".
3. Expression simplification.
4. Proper parser errors.
