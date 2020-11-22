#pragma once
#include "Expression.h"

#include <memory>

class BinaryExpression : public Expression
{
public:
    BinaryExpression(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : lhs_(std::move(lhs))
        , rhs_(std::move(rhs))
    {}

protected:
    std::shared_ptr<Expression> lhs_;
    std::shared_ptr<Expression> rhs_;
};
