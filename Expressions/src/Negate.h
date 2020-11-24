#pragma once

#include "Expression.h"

class Negate final : public Expression
{
public:
    Negate(std::shared_ptr<Expression> expr) : expr_(std::move(expr)) {};

    std::shared_ptr<Expression> diff(const std::string_view var) const override
    {
        return std::make_shared<Negate>(expr_->diff(var));
    }

    std::string tostring() const override
    {
        return "--" + expr_->tostring();
    }

    double evaluate(double x) const override
    {
        return -expr_->evaluate(x);
    }

    bool contains_var(const std::string_view var) const override
    {
        return expr_->contains_var(var);
    }

private:
    std::shared_ptr<Expression> expr_;
};