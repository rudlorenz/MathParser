#pragma once

#include "Expression.h"

class Number final : public Expression
{
public:
    Number() : value_() {};
    explicit Number(const int& value) : value_(value) {};

    std::shared_ptr<Expression> diff(const std::string_view) const override { return std::make_shared<Number>(0); }

    std::string to_string() const override { return fmt::format("{}", value_); }

    std::string to_expr_string() const override { return fmt::format("Num({})", value_); }

    double evaluate(double) const override { return value_; }

    bool contains_var(const std::string_view) const override { return false; }

private:
    int value_;
};
