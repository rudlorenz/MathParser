#pragma once

#include "Expression.h"
#include "Mul.h"
#include "Sub.h"

#include <cmath>

class Cos;

class Sin : public Expression
{
public:
    explicit Sin(std::shared_ptr<Expression> value) : value_(std::move(value)) {};

    std::shared_ptr<Expression> diff() const override
    {
        return std::make_shared<Mul>(value_->diff(), std::make_shared<Cos>(value_));
    }

    std::string tostring() const override
    {
        return "sin(" + value_->tostring() + ")";
    }
    double evaluate(double x) const override
    {
        return sin(x);
    }

private:
    std::shared_ptr<Expression> value_;
};

class Cos : public Expression
{
public:
    explicit Cos(std::shared_ptr<Expression> value): value_(std::move(value)) {};

    std::shared_ptr<Expression> diff() const override
    {
        return std::make_shared<Sum>(
            value_->diff(),
            std::make_shared<Sub>(std::make_shared<Number>(0), std::make_shared<Sin>(value_))
        );
    }

    std::string tostring() const override
    {
        return "cos(" + value_->tostring() + ")";
    }

    double evaluate(double x) const override
    {
        return cos(x);
    }


private:
    std::shared_ptr<Expression> value_;
};

