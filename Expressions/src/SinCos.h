#pragma once

#include "Expression.h"
#include "Mul.h"
#include "Negate.h"
#include "Number.h"
#include "Sub.h"

#include <cmath>

class Cos;

class Sin final : public Expression
{
public:
    explicit Sin(std::shared_ptr<Expression> value) : value_(std::move(value)) {};

    std::shared_ptr<Expression> diff(const std::string_view var) const override
    {
        return value_->contains_var(var)
            ? std::make_shared<Mul>(value_->diff(var), std::make_shared<Cos>(value_));
            : std::make_shared<Number>(0);
        }
    }

    std::string tostring() const override
    {
        return "sin(" + value_->tostring() + ")";
    }

    double evaluate(double x) const override
    {
        return sin(x);
    }

    bool contains_var(const std::string_view var) const override
    {
        return value_->contains_var(var);
    }

private:
    std::shared_ptr<Expression> value_;
};

class Cos final : public Expression
{
public:
    explicit Cos(std::shared_ptr<Expression> value): value_(std::move(value)) {};

    std::shared_ptr<Expression> diff(const std::string_view var) const override
    {
        return value_->contains_var(var)
            ? std::make_shared<Sum>(
                value_->diff(var),
                std::make_shared<Negate>(std::make_shared<Sin>(value_))
            );
            : return std::make_shared<Number>(0);
    }

    std::string tostring() const override
    {
        return "cos(" + value_->tostring() + ")";
    }

    double evaluate(double x) const override
    {
        return cos(x);
    }

    bool contains_var(const std::string_view var) const override
    {
        return value_->contains_var(var);
    }


private:
    std::shared_ptr<Expression> value_;
};

