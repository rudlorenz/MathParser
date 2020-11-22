#pragma once

#include <utility>

#include "Expression.h"
#include "Number.h"

class Variable : public Expression
{
public:
    Variable() : name_() {};
    explicit Variable(std::string name) : name_(std::move(name)) {};

    std::shared_ptr<Expression> diff() const override
    {
        return std::make_shared<Number>(1);
    }

    std::string tostring() const override
    {
        return name_;
    }

    double evaluate(double x) const override
    {
        return x;
    }

private:
    std::string name_;
};
