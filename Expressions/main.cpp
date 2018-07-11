#include <iostream>
#include "Expression.h"
#include "Sum.h"
#include "Variable.h"
#include "Sub.h"
#include "Mul.h"
#include "Div.h"
#include "SinCos.h"

int main()
{
    //sin^2(a) + cos^2(a)
    Expression* test_complex = new Sum(
            new Mul(new Sin(new Variable("a")), new Sin(new Variable("a"))),
            new Mul(new Cos(new Variable("a")), new Cos(new Variable("a")))
    );

    std::cout << test_complex->evaluate(1) << "\n";
    std::cout << test_complex->evaluate(5) << "\n";
    std::cout << test_complex->evaluate(101) << "\n";

    std::cout << test_complex->diff()->tostring() << "\n";

    return 0;
}