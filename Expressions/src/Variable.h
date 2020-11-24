#pragma once

#include "Expression.h"
#include "Number.h"

#include <utility>

class Variable : public Expression
{
public:
    Variable() : name_() {};
    explicit Variable(std::string name) : name_(std::move(name)) {};

    std::shared_ptr<Expression> diff(const std::string_view) const override
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

    bool contains_var(const std::string_view var) const override
    {
        return name_ == var;
    }

private:
    std::string name_;
};
