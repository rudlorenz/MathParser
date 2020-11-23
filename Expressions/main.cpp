#include <algorithm>
#include <iostream>

#include "Expression.h"

#include "Parser.h"

int main()
{
    std::string input = "(a + b * c) / 10";
    auto result = parse(input);
    if (result == nullptr) {
        std::cout << "Something wrong!";
        return 0;
    }

    std::cout << input << "\n";
    std::cout << result->tostring() << "\n";
    std::cout << result->diff()->tostring() << std::endl;

    return 0;
}