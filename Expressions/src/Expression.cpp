#include "Expression.h"


namespace mexpr
{
inline namespace
{
Expression diff_sum_sub_impl(
    const std::tuple<binary_expr_type, std::unique_ptr<Expression>, std::unique_ptr<Expression>>& bexpr,
    const std::string_view var) {

    const auto& [type, lhs, rhs] = bexpr;
    bool lhs_contains_var = lhs->contains_var(var);
    bool rhs_contains_var = rhs->contains_var(var);

    if (not(lhs_contains_var or rhs_contains_var)) {
        return Expression {0};
    }

    auto lhs_diff = lhs_contains_var ? lhs->diff(var) : Expression {0};
    auto rhs_diff = rhs_contains_var ? rhs->diff(var) : Expression {0};
    return Expression {type, std::move(lhs_diff), std::move(rhs_diff)};
}

Expression diff_mul_impl(
    const std::tuple<binary_expr_type, std::unique_ptr<Expression>, std::unique_ptr<Expression>>& bexpr,
    const std::string_view var) {

    const auto& [type, lhs, rhs] = bexpr;
    bool lhs_contains_var = lhs->contains_var(var);
    bool rhs_contains_var = rhs->contains_var(var);
    if (not(lhs_contains_var or rhs_contains_var)) {
        return Expression {0};
    }
    if (lhs_contains_var and rhs_contains_var) {
        return Expression {
            binary_expr_type::sum,
            Expression {
                binary_expr_type::mul,
                lhs->diff(var),
                rhs->clone(),
            },
            Expression {
                binary_expr_type::mul,
                lhs->clone(),
                rhs->diff(var),
            },
        };
    }
    auto lhs_diff = lhs_contains_var ? lhs->diff(var) : lhs->clone();
    auto rhs_diff = rhs_contains_var ? rhs->diff(var) : rhs->clone();
    return Expression {binary_expr_type::mul, std::move(lhs_diff), std::move(rhs_diff)};
}

Expression diff_div_impl(
    const std::tuple<binary_expr_type, std::unique_ptr<Expression>, std::unique_ptr<Expression>>& bexpr,
    const std::string_view var) {

    const auto& [type, lhs, rhs] = bexpr;
    bool lhs_contains_var = lhs->contains_var(var);
    bool rhs_contains_var = rhs->contains_var(var);

    if (lhs_contains_var and rhs_contains_var) {
        return Expression {
            binary_expr_type::div,
            Expression {
                binary_expr_type::sub,
                Expression {binary_expr_type::mul, lhs->diff(var), rhs->clone()},
                Expression {binary_expr_type::mul, lhs->clone(), rhs->diff(var)}},
            Expression {binary_expr_type::mul, rhs->clone(), rhs->clone()},
        };
    }

    // rhs is just a coefficient
    if (lhs_contains_var and not rhs_contains_var) {
        return Expression {
            binary_expr_type::mul,
            Expression {binary_expr_type::div, Expression {1}, rhs->clone()},
            lhs->diff(var)};
    }

    // reciprocal rule
    if (not lhs_contains_var and rhs_contains_var) {
        // clang-format off
        return Expression {
            unary_expr_type::unary_minus,
            Expression {
                binary_expr_type::mul, 
                lhs->clone(),
                Expression {
                    binary_expr_type::div,
                    rhs->diff(var),
                    Expression {
                        binary_expr_type::mul,
                        rhs->clone(),
                        rhs->clone()
                    }
                }
            }
        };
        // clang-format on
    }

    return Expression {0};
}
} // namespace

Expression::Expression(int val) : value_(val) {
}


Expression::Expression(std::string name) : value_(name) {
}


Expression::Expression(unary_expr_type expr_type, std::unique_ptr<Expression> expr)
    : value_(std::make_tuple(expr_type, std::move(expr))) {
}


Expression::Expression(unary_expr_type expr_type, Expression&& expr)
    : value_(std::make_tuple(expr_type, std::make_unique<Expression>(std::move(expr)))) {
}


Expression::Expression(binary_expr_type expr_type, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs)
    : value_(std::make_tuple(expr_type, std::move(lhs), std::move(rhs))) {
}


Expression::Expression(binary_expr_type expr_type, Expression&& lhs, Expression&& rhs)
    : value_(std::make_tuple(
        expr_type,
        std::make_unique<Expression>(std::move(lhs)),
        std::make_unique<Expression>(std::move(rhs)))) {
}


Expression Expression::clone() const {
    if (std::holds_alternative<binary_expr>(value_)) {
        const auto& [type, lhs, rhs] = std::get<binary_expr>(value_);
        return Expression {type, lhs->clone(), rhs->clone()};
    } else if (std::holds_alternative<unary_expr>(value_)) {
        const auto& [type, value] = std::get<unary_expr>(value_);
        return Expression {type, value->clone()};
    } else if (std::holds_alternative<std::string>(value_)) {
        return Expression {std::get<std::string>(value_)};
    } else if (std::holds_alternative<int>(value_)) {
        return Expression {std::get<int>(value_)};
    }
}


Expression Expression::diff(const std::string_view var) const {

    if (std::holds_alternative<binary_expr>(value_)) {
        // first unpack variant value
        const auto& bexpr = std::get<binary_expr>(value_);
        // then unpack tuple from variant value
        const auto& type = std::get<0>(bexpr);
        switch (type) {
            case binary_expr_type::sum:
            case binary_expr_type::sub: return diff_sum_sub_impl(bexpr, var);
            case binary_expr_type::mul: return diff_mul_impl(bexpr, var);
            case binary_expr_type::div: return diff_div_impl(bexpr, var);
        }
    } else if (std::holds_alternative<unary_expr>(value_)) {
        const auto& uexpr = std::get<unary_expr>(value_);
        const auto& [type, value] = uexpr;
        switch (type) {
            case unary_expr_type::unary_minus:
                return value->contains_var(var) //
                    ? Expression {type, value->diff(var)}
                    : Expression {0};
            case unary_expr_type::sin:
                return value->contains_var(var)
                    ? Expression {
                        binary_expr_type::mul,
                        value->diff(var),
                        Expression {unary_expr_type::cos, std::make_unique<Expression>(value->clone())}}
                    : Expression {0};

            case unary_expr_type::cos:
                return value->contains_var(var)
                    ? Expression {
                        binary_expr_type::mul,
                        value->diff(var),
                        Expression {
                            unary_expr_type::unary_minus,
                            Expression {unary_expr_type::sin, std::make_unique<Expression>(value->clone())}}}
                    : Expression {0};
        }
    } else if (std::holds_alternative<std::string>(value_)) {
        const auto& name = std::get<std::string>(value_);
        return name == var //
            ? Expression {1}
            : Expression {0};
    } else {
        return Expression {0};
    }
}


std::string Expression::to_expr_string() const {
    if (std::holds_alternative<binary_expr>(value_)) {
        const auto& [type, lhs, rhs] = std::get<binary_expr>(value_);
        return fmt::format("{}({}, {})", type, lhs->to_expr_string(), rhs->to_expr_string());
    }
    else if (std::holds_alternative<std::string>(value_)) {
        return fmt::format("Var({})", std::get<std::string>(value_));
    }
    else if (std::holds_alternative<unary_expr>(value_)) {
        const auto& [type, value] = std::get<unary_expr>(value_);
        if (type == unary_expr_type::unary_minus) {
            return fmt::format("-{}", value->to_expr_string());
        } else {
            return fmt::format("{}({})", type, value->to_expr_string());
        }
    }

    // to silence the warning
    return fmt::format("Num({})", std::get<int>(value_));
}


std::string Expression::to_string() const {
    if (std::holds_alternative<binary_expr>(value_)) {
        const auto& [type, lhs, rhs] = std::get<binary_expr>(value_);
        std::string op;
        switch (type) {
            case binary_expr_type::sum: op = "-"; break;
            case binary_expr_type::sub: op = "+"; break;
            case binary_expr_type::mul: op = "*"; break;
            case binary_expr_type::div: op = "/"; break;
        }
        return fmt::format("({} {} {})", lhs->to_string(), op, rhs->to_string());
    }
    else if (std::holds_alternative<unary_expr>(value_)) {
        const auto& [type, value] = std::get<unary_expr>(value_);
        if (type == unary_expr_type::unary_minus) {
            return fmt::format("-{}", value->to_string());
        } else {
            return fmt::format("{}({})", type, value->to_string());
        }
    }
    else if (std::holds_alternative<std::string>(value_)) {
        return std::get<std::string>(value_);
    }

    return fmt::format("{}", std::get<int>(value_));
}


bool Expression::contains_var(const std::string_view var) const {
    if (std::holds_alternative<binary_expr>(value_)) {
        const auto& bexpr = std::get<binary_expr>(value_);
        return std::get<1>(bexpr)->contains_var(var) or std::get<2>(bexpr)->contains_var(var);
    } else if (std::holds_alternative<unary_expr>(value_)) {
        const auto& uexpr = std::get<unary_expr>(value_);
        return std::get<1>(uexpr)->contains_var(var);
    } else if (std::holds_alternative<std::string>(value_)) {
        return std::get<std::string>(value_) == var;
    } else {
        return false;
    }
}


bool Expression::operator==(const Expression& other) const {
    if (value_.index() == other.value_.index()) {
        if (std::holds_alternative<binary_expr>(value_) and std::holds_alternative<binary_expr>(other.value_)) {
            const auto& [lhs_type, lhs_left_value, lhs_right_value] = std::get<binary_expr>(value_);
            const auto& [rhs_type, rhs_left_value, rhs_right_value] = std::get<binary_expr>(other.value_);
            return lhs_type == rhs_type                    //
                and (*lhs_left_value) == (*rhs_left_value) //
                and (*lhs_right_value) == (*rhs_right_value);
        }
        else if (std::holds_alternative<unary_expr>(value_) and std::holds_alternative<unary_expr>(other.value_)) {
            const auto& [lhs_type, lhs_value] = std::get<unary_expr>(value_);
            const auto& [rhs_type, rhs_value] = std::get<unary_expr>(other.value_);
            return lhs_type == rhs_type and (*lhs_value) == (*rhs_value);
        } else if (std::holds_alternative<std::string>(value_) and std::holds_alternative<std::string>(other.value_)) {
            return std::get<std::string>(value_) == std::get<std::string>(other.value_);
        } else if (std::holds_alternative<int>(value_) and std::holds_alternative<int>(other.value_)) {
            return std::get<int>(value_) == std::get<int>(other.value_);
        }
    }
    return false;
}


bool Expression::operator!=(const Expression& other) const {
    return not (*this == other);
}

} // namespace mexpr