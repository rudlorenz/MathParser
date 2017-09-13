#ifndef EXPRESSIONS_BINARYEXPRESSION_H
#define EXPRESSIONS_BINARYEXPRESSION_H


#include "Expression.h"

class BinaryExpression : public Expression
{
public:
    virtual ~BinaryExpression();

protected:
    Expression*  first_operand_;
    Expression* second_operand_;
};

BinaryExpression::~BinaryExpression()
{
    delete first_operand_;
    delete second_operand_;
}


#endif //EXPRESSIONS_BINARYEXPRESSION_H
