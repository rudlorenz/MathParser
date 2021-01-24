#pragma once
#include <memory>
#include <string>
#include <string_view>

class Expression
{
public:
    virtual std::shared_ptr<Expression> diff(std::string_view var) const = 0;
    virtual std::string tostring() const = 0;
    virtual double evaluate(double x) const = 0;
    virtual bool contains_var(std::string_view var) const = 0;

    virtual ~Expression() = default;
};
