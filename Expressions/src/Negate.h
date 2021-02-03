#pragma once

#include "Expression.h"

class Negate final : public Expression
{
public:
    Negate(std::shared_ptr<Expression> expr) : expr_(std::move(expr)) {};

    std::shared_ptr<Expression> diff(const std::string_view var) const override {
        return std::make_shared<Negate>(expr_->diff(var));
    }

    // em dash to represent unary minus.
    std::string to_string() const override { return fmt::format("—{}", expr_->to_string()); }

    std::string to_expr_string() const override { return fmt::format("neg({})", expr_->to_expr_string()); }

    double evaluate(double x) const override { return -expr_->evaluate(x); }

    bool contains_var(const std::string_view var) const override { return expr_->contains_var(var); }

private:
    std::shared_ptr<Expression> expr_;
};
