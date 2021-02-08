#pragma once

#include "Expression.h"
#include "Number.h"

#include <utility>

class Variable final : public Expression
{
public:
    Variable() : name_() {};
    explicit Variable(std::string name) : name_(std::move(name)) {};

    std::shared_ptr<Expression> diff(const std::string_view name) const override {
        if (name_ == name) {
            return std::make_shared<Number>(1);
        } else {
            return std::make_shared<Number>(0);
        };
    }

    std::string to_string() const override { return name_; }

    std::string to_expr_string() const override { return fmt::format("var({})", name_); }

    double evaluate(double x) const override { return x; }

    bool contains_var(const std::string_view var) const override { return name_ == var; }

private:
    std::string name_;
};
