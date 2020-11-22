#pragma once

#include "Expression.h"

class Number : public Expression
{
public:
    Number() : value_() {};
    explicit Number(const int& value) : value_(value) {};

    std::shared_ptr<Expression> diff() const override
    {
        return std::make_shared<Number>(0);
    }

    std::string tostring() const override
    {
        return std::to_string(value_);
    }

    double evaluate(double) const override
    {
        return value_;
    }

private:
    int value_;
};
