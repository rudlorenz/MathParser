#pragma once

#include "BinaryExpression.h"
#include "Sub.h"
#include "Mul.h"

class Div : public BinaryExpression
{
public:
    Div(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : BinaryExpression(std::move(lhs), std::move(rhs))
    {
    }

    std::shared_ptr<Expression> diff() const override
    {
        return std::make_shared<Div>(
            std::make_shared<Sub>(
                std::make_shared<Mul>(lhs_->diff(), rhs_),
                std::make_shared<Mul>(lhs_, rhs_->diff())
            ),
            std::make_shared<Mul>(rhs_, rhs_));
    }

    std::string tostring() const override
    {
        return "(" + lhs_->tostring() + " / " + rhs_->tostring() + ")";;
    }

    double evaluate(double x) const override
    {
        return lhs_->evaluate(x) / rhs_->evaluate(x);
    }
};
