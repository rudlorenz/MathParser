#include "Expressions.h"
#include "Parser.h"

#include "fmt/core.h"
#include "fmt/format.h"

int main()
{
    std::string input = "-x*x/y";
    auto result = parser::parse(input);
    if (result == nullptr) {
        fmt::print(stderr, "Something wrong!");
        return 0;
    }

    fmt::print(stdout, "input : {}\n", input);
    fmt::print(stdout, "as expressions : {}\n", result->tostring());
    fmt::print(stdout, "d/dx : {}\n", result->diff("x")->tostring());

    return 0;
}