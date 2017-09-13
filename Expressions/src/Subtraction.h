#ifndef EXPRESSIONS_SUBTRACTION_H
#define EXPRESSIONS_SUBTRACTION_H


#include "BinaryExpression.h"

class Subtraction : public BinaryExpression
{
public:
    Subtraction(Expression* first_operand, Expression* second_operand);

    Expression *diff() const override;
    std::string tostring() const override;
};

Subtraction::Subtraction(Expression *first_operand, Expression *second_operand)
{
    first_operand_ = first_operand;
    second_operand_ = second_operand;
}

Expression *Subtraction::diff() const
{
    return new Subtraction(first_operand_->diff(), second_operand_->diff());
}

std::string Subtraction::tostring() const
{
    return "(" + first_operand_->tostring() + " - " + second_operand_->tostring() + ")";
}

#endif //EXPRESSIONS_SUBTRACTION_H