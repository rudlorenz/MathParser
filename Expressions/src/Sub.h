#ifndef EXPRESSIONS_SUBTRACTION_H
#define EXPRESSIONS_SUBTRACTION_H


#include "BinaryExpression.h"

class Sub : public BinaryExpression
{
public:
    Sub(Expression* first_operand, Expression* second_operand);

    Expression *diff() const override;
    std::string tostring() const override;
    double evaluate(double x) const override;

};

Sub::Sub(Expression *first_operand, Expression *second_operand)
{
    first_operand_ = first_operand;
    second_operand_ = second_operand;
}

Expression *Sub::diff() const
{
    return new Sub(first_operand_->diff(), second_operand_->diff());
}

std::string Sub::tostring() const
{
    return "(" + first_operand_->tostring() + " - " + second_operand_->tostring() + ")";
}

double Sub::evaluate(double x) const
{
    return first_operand_->evaluate(x) - second_operand_->evaluate(x);
}

#endif //EXPRESSIONS_SUBTRACTION_H