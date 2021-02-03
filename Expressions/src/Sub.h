#pragma once

#include "BinaryExpression.h"
#include "Number.h"

class Sub final : public BinaryExpression
{
public:
    Sub(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : BinaryExpression(std::move(lhs), std::move(rhs)) {}

    std::shared_ptr<Expression> diff(const std::string_view var) const override {
        if (!lhs_->contains_var(var) && !rhs_->contains_var(var)) {
            return std::make_shared<Number>(0);
        }
        auto lhs = lhs_->contains_var(var) ? lhs_->diff(var) : std::make_shared<Number>(0);

        auto rhs = rhs_->contains_var(var) ? rhs_->diff(var) : std::make_shared<Number>(0);

        return std::make_shared<Sub>(lhs, rhs);
    }

    std::string to_string() const override { return fmt::format("({} - {})", lhs_->to_string(), rhs_->to_string()); }

    std::string to_expr_string() const override { return fmt::format("sub({}, {})", lhs_->to_expr_string(), rhs_->to_expr_string()); }

    double evaluate(double x) const override { return lhs_->evaluate(x) - rhs_->evaluate(x); }
};
