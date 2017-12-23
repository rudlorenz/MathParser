#ifndef EXPRESSIONS_MUL_H
#define EXPRESSIONS_MUL_H


#include "BinaryExpression.h"
#include "Sum.h"

class Mul : public BinaryExpression
{
public:
    Mul(Expression* first_operand, Expression* second_operand);

    Expression *diff() const override;
    std::string tostring() const override;
    double evaluate(double x) const override;

};

Mul::Mul(Expression *first_operand, Expression *second_operand)
{
    first_operand_  = first_operand;
    second_operand_ = second_operand;

}

Expression *Mul::diff() const
{
    return new Sum(
            new Mul(first_operand_->diff(), second_operand_),
            new Mul(first_operand_, second_operand_->diff())
    );
}

std::string Mul::tostring() const
{
    return "(" + first_operand_->tostring() + " * " + second_operand_->tostring() + ")";;
}

double Mul::evaluate(double x) const
{
    return first_operand_->evaluate(x) * second_operand_->evaluate(x);
}

#endif //EXPRESSIONS_MUL_H
