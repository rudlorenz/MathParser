#ifndef EXPRESSIONS_SUM_H
#define EXPRESSIONS_SUM_H


#include "BinaryExpression.h"

class Sum : public BinaryExpression
{
public:
    Sum(Expression* first_operand, Expression* second_operand);

    Expression *diff() const override;
    std::string tostring() const override;

};

Expression *Sum::diff() const
{
    return new Sum(first_operand_->diff(), second_operand_->diff());
}

std::string Sum::tostring() const
{
    return "(" + first_operand_->tostring() + " + " + second_operand_->tostring() + ")";
}

Sum::Sum(Expression *first_operand, Expression *second_operand)
{
    first_operand_ = first_operand;
    second_operand_ = second_operand;
}

#endif //EXPRESSIONS_SUM_H