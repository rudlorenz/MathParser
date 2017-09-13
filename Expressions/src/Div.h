#ifndef EXPRESSIONS_DIV_H
#define EXPRESSIONS_DIV_H


#include "BinaryExpression.h"
#include "Sub.h"
#include "Mul.h"

class Div : public BinaryExpression
{
public:
    Div(Expression* first_operand, Expression* second_operand);

    Expression *diff() const override;
    std::string tostring() const override;
};

Div::Div(Expression *first_operand, Expression *second_operand)
{
    first_operand_  = first_operand;
    second_operand_ = second_operand;
}

Expression *Div::diff() const
{
    return new Div(
            new Sub (new Mul(first_operand_->diff(), second_operand_),
                     new Mul(first_operand_, second_operand_->diff()) ),
            new Mul(second_operand_, second_operand_)
    );
}

std::string Div::tostring() const
{
    return "(" + first_operand_->tostring() + " / " + second_operand_->tostring() + ")";;
}


#endif //EXPRESSIONS_DIV_H
