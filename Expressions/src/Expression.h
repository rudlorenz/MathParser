#pragma once
#include <string>
#include <string_view>
#include <memory>

class Expression
{
public:
    virtual std::shared_ptr<Expression> diff(const std::string_view var) const = 0;
    virtual std::string tostring() const = 0;
    virtual double evaluate(double x) const = 0;
    virtual bool contains_var(const std::string_view var) const = 0;

    virtual ~Expression() = default;
};
