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
    const auto test_complex = std::make_shared<Sum>(
        std::make_shared<Mul>(std::make_shared<Sin>(std::make_shared<Variable>("a")), std::make_shared<Sin>(std::make_shared<Variable>("a"))),
        std::make_shared<Mul>(std::make_shared<Cos>(std::make_shared<Variable>("a")), std::make_shared<Cos>(std::make_shared<Variable>("a")))
    );

    std::cout << test_complex->evaluate(1) << "\n";
    std::cout << test_complex->evaluate(5) << "\n";
    std::cout << test_complex->evaluate(101) << "\n";

    std::cout << test_complex->diff()->tostring() << "\n";

    return 0;
}