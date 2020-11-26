#include <algorithm>
#include <iostream>

#include "Expression.h"

#include "Parser.h"

int main()
{
    std::string input = "-x*x/y";
    auto result = parse(input);
    if (result == nullptr) {
        std::cout << "Something wrong!";
        return 0;
    }

    std::cout << input << "\n";
    std::cout << result->tostring() << "\n";
    std::cout << result->diff("x")->tostring() << std::endl;

    return 0;
}