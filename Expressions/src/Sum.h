#pragma once

#include "BinaryExpression.h"

class Sum : public BinaryExpression
{
public:
    Sum(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : BinaryExpression(std::move(lhs), std::move(rhs))
    {
    }

    std::shared_ptr<Expression> diff() const override
    {
        return std::make_shared<Sum>(lhs_->diff(), rhs_->diff());
    }

    std::string tostring() const override
    {
        return "(" + lhs_->tostring() + " + " + rhs_->tostring() + ")";
    }

    double evaluate(double x) const override
    {
        return lhs_->evaluate(x) + rhs_->evaluate(x);
    }

};
