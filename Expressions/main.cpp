#include <iostream>
#include "src/Expression.h"
#include "src/Sum.h"
#include "src/Variable.h"
#include "src/Subtraction.h"

int main()
{
    Expression* test_sum = new Sum(new Variable("a"), new Variable("b"));
    Expression* test_sub = new Subtraction(new Number(5), new Variable("c"));

    std::cout << test_sum->tostring() << std::endl;
    std::cout << test_sum->diff()->tostring() << std::endl;

    std::cout << "\n"
              << test_sub->tostring() << std::endl;
    std::cout << test_sub->diff()->tostring() << std::endl;

    return 0;
}