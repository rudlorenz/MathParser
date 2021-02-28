#pragma once

#include <fmt/core.h>
#include <fmt/format.h>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>

namespace mexpr
{

enum class unary_expr_type
{
    sin,
    cos,
    unary_minus,
};

enum class binary_expr_type
{
    sum,
    sub,
    div,
    mul,
};

class Expression
{
    using unary_expr = std::tuple<unary_expr_type, std::unique_ptr<Expression>>;
    using binary_expr = std::tuple<binary_expr_type, std::unique_ptr<Expression>, std::unique_ptr<Expression>>;

public:
    explicit Expression(int val);
    explicit Expression(std::string name);
    Expression(unary_expr_type expr_type, std::unique_ptr<Expression> expr);
    Expression(unary_expr_type expr_type, Expression&& expr);
    Expression(binary_expr_type expr_type, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
    Expression(binary_expr_type expr_type, Expression&& lhs, Expression&& rhs);

    bool operator==(const Expression& other) const;
    bool operator!=(const Expression& other) const;

public:
    [[nodiscard]] Expression clone() const;
    [[nodiscard]] Expression diff(std::string_view var) const;

    std::string to_expr_string() const;
    std::string to_string() const;

    bool contains_var(std::string_view var) const;

private:
    std::variant<int, std::string, unary_expr, binary_expr> value_;
};
} // namespace mexpr

template <>
struct fmt::formatter<mexpr::unary_expr_type> : formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(mexpr::unary_expr_type expr_type, FormatContext& ctx) {
        switch (expr_type) {
            case mexpr::unary_expr_type::unary_minus: return formatter<string_view>::format(std::string_view("-"), ctx);
            case mexpr::unary_expr_type::sin: return formatter<string_view>::format(std::string_view("sin"), ctx);
            case mexpr::unary_expr_type::cos: return formatter<string_view>::format(std::string_view("cos"), ctx);
            default: return formatter<string_view>::format(std::string_view("unknown"), ctx);
        }
    }
};


template <>
struct fmt::formatter<mexpr::binary_expr_type> : formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(mexpr::binary_expr_type expr_type, FormatContext& ctx) {
        switch (expr_type) {
            case mexpr::binary_expr_type::sum: return formatter<string_view>::format(std::string_view("sum"), ctx);
            case mexpr::binary_expr_type::sub: return formatter<string_view>::format(std::string_view("sub"), ctx);
            case mexpr::binary_expr_type::div: return formatter<string_view>::format(std::string_view("div"), ctx);
            case mexpr::binary_expr_type::mul: return formatter<string_view>::format(std::string_view("mul"), ctx);
            default: return formatter<string_view>::format(std::string_view("unknown"), ctx);
        }
    }
};
