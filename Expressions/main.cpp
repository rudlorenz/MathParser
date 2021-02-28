#include "Parser.h"

#include "fmt/core.h"

int main()
{
    std::string input = "-x*x/y";
    auto result = parser::parse(input);
    if (!result) {
        fmt::print(stderr, "Something wrong!");
        return 0;
    }

    fmt::print(stdout, "input : {}\n", input);
    fmt::print(stdout, "as expressions : {}\n", result->to_expr_string());
    fmt::print(stdout, "d/dx : {}\n", result->diff("x").to_string());

    return 0;
}