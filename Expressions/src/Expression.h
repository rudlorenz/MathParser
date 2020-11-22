#pragma once
#include <string>
#include <memory>

class Expression
{
public:
    virtual std::shared_ptr<Expression> diff() const = 0;
    virtual std::string tostring() const = 0;
    virtual double evaluate(double x) const = 0;

    virtual ~Expression() = default;
};
