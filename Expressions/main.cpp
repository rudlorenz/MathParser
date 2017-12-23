#include <iostream>
#include "src/Expression.h"
#include "src/Sum.h"
#include "src/Variable.h"
#include "src/Sub.h"
#include "src/Mul.h"
#include "src/Div.h"
#include "src/SinCos.h"

int main()
{
    Expression* test_sum = new Sum(new Variable("a"), new Variable("b"));
    Expression* test_sub = new Sub(new Number(5), new Variable("c"));
    Expression* test_mul = new Mul(new Variable("a"), new Variable("b"));
    Expression* test_div = new Div(new Variable("a"), new Variable("b"));
    Expression* test_sin = new Sin(new Variable("a"));
    Expression* test_cos = new Cos(new Variable("b"));

    std::cout << test_sum->tostring() << std::endl;
    std::cout << test_sum->diff()->tostring() << std::endl;

    std::cout << "\n"
              << test_sub->tostring() << std::endl;
    std::cout << test_sub->diff()->tostring() << std::endl;

    std::cout << "\n"
              << test_mul->tostring() << std::endl;
    std::cout << test_mul->diff()->tostring() << std::endl;

    std::cout << "\n"
              << test_div->tostring() << std::endl;
    std::cout << test_div->diff()->tostring() << std::endl;

    std::cout << "\n"
              << test_sin->tostring() << std::endl;
    std::cout << test_sin->diff()->tostring() << std::endl;

    std::cout << "\n"
              << test_cos->tostring() << std::endl;
    std::cout << test_cos->diff()->tostring() << std::endl;

    std::cout << "\n\n";

    Expression* test_sum_eval = new Sum(new Variable("a"), new Number(5));
    std::cout << test_sum_eval->evaluate(10) << std::endl;

    Expression* test_sub_eval = new Sub(new Variable("a"), new Number(5));
    std::cout << test_sub_eval->evaluate(10) << std::endl;

    Expression* test_div_eval = new Div(new Variable("a"), new Number(5));
    std::cout << test_div_eval->evaluate(10) << std::endl;

    Expression* test_mul_eval = new Mul(new Variable("a"), new Number(5));
    std::cout << test_mul_eval->evaluate(10) << std::endl;

    return 0;
}