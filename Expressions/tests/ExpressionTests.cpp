#include "Expression.h"

#include "gtest/gtest.h"

namespace mexpr
{

// Required to "pretty print" expression values in case of test failure
std::ostream& operator<<(std::ostream& os, const Expression& expr) {
    return os << expr.to_expr_string();
}
} // namespace mexpr

using namespace mexpr;

class ExpressionTests : public ::testing::Test
{
public:
    ExpressionTests() : one_expr {1}, two_expr {2}, zero_expr {0}, var_a {"a"}, var_b {"b"} {};

protected:
    const Expression one_expr;
    const Expression two_expr;
    const Expression zero_expr;
    const Expression var_a;
    const Expression var_b;
};

TEST_F(ExpressionTests, basic_tests) {
    EXPECT_EQ(one_expr, Expression {1});
    EXPECT_EQ(zero_expr, Expression {0});
    EXPECT_EQ(var_a, Expression {"a"});
    EXPECT_EQ(var_b, Expression {"b"});

    EXPECT_EQ(zero_expr, zero_expr.clone());
    EXPECT_EQ(one_expr, one_expr.clone());
    EXPECT_EQ(var_a, var_a.clone());


    EXPECT_NE(one_expr, zero_expr);
    EXPECT_NE(var_a, var_b);
    EXPECT_NE(one_expr, var_a);
    EXPECT_NE(zero_expr, var_b);
}

TEST_F(ExpressionTests, basic_diff) {
    EXPECT_EQ(zero_expr, zero_expr.diff("a"));
    EXPECT_EQ(zero_expr, zero_expr.diff("1"));
    EXPECT_EQ(zero_expr, zero_expr.diff(""));
    EXPECT_EQ(zero_expr, zero_expr.diff("x"));

    EXPECT_EQ(zero_expr, one_expr.diff("a"));
    EXPECT_EQ(zero_expr, one_expr.diff("1"));
    EXPECT_EQ(zero_expr, one_expr.diff(""));
    EXPECT_EQ(zero_expr, one_expr.diff("x"));

    EXPECT_EQ(one_expr, var_a.diff("a"));
    EXPECT_EQ(one_expr, var_b.diff("b"));

    EXPECT_EQ(zero_expr, var_a.diff("b"));
    EXPECT_EQ(zero_expr, var_a.diff("1"));
    EXPECT_EQ(zero_expr, var_a.diff(""));
    EXPECT_EQ(zero_expr, var_a.diff("x"));
}

TEST_F(ExpressionTests, simple_sum_tests) {

    {
        const auto one_plus_one = Expression {binary_expr_type::sum, one_expr.clone(), one_expr.clone()};
        EXPECT_EQ(zero_expr, one_plus_one.diff(""));
        EXPECT_EQ(zero_expr, one_plus_one.diff("1"));
        EXPECT_EQ(zero_expr, one_plus_one.diff("a"));
        EXPECT_EQ(zero_expr, one_plus_one.diff("b"));
    }

    {
        const auto zero_plus_one = Expression {binary_expr_type::sum, zero_expr.clone(), one_expr.clone()};
        const auto one_plus_a = Expression {binary_expr_type::sum, one_expr.clone(), var_a.clone()};
        EXPECT_EQ(zero_expr, one_plus_a.diff(""));
        EXPECT_EQ(zero_expr, one_plus_a.diff("1"));
        EXPECT_EQ(zero_expr, one_plus_a.diff("b"));

        const auto one_plus_b = Expression {binary_expr_type::sum, one_expr.clone(), var_b.clone()};
        EXPECT_EQ(zero_plus_one, one_plus_a.diff("a"));
        EXPECT_EQ(zero_plus_one, one_plus_b.diff("b"));
    }

    {
        const auto a_plus_b = Expression {binary_expr_type::sum, var_a.clone(), var_b.clone()};
        EXPECT_EQ(zero_expr, a_plus_b.diff(""));

        const auto one_plus_zero = Expression {binary_expr_type::sum, one_expr.clone(), zero_expr.clone()};
        EXPECT_EQ(one_plus_zero, a_plus_b.diff("a"));

        const auto zero_plus_one = Expression {binary_expr_type::sum, zero_expr.clone(), one_expr.clone()};
        EXPECT_EQ(zero_plus_one, a_plus_b.diff("b"));
    }
}

TEST_F(ExpressionTests, simple_mul_tests) {
    {
        const auto one_mul_one = Expression {binary_expr_type::mul, one_expr.clone(), one_expr.clone()};
        EXPECT_EQ(zero_expr, one_mul_one.diff(""));
        EXPECT_EQ(zero_expr, one_mul_one.diff("a"));
        EXPECT_EQ(zero_expr, one_mul_one.diff("b"));
    }
    {
        const auto two_mul_a = Expression {binary_expr_type::mul, Expression {2}, var_a.clone()};
        EXPECT_EQ(zero_expr, two_mul_a.diff(""));
        EXPECT_EQ(zero_expr, two_mul_a.diff("1"));
        EXPECT_EQ(zero_expr, two_mul_a.diff("2"));
        EXPECT_EQ(zero_expr, two_mul_a.diff("b"));

        const auto two_mul_one = Expression {binary_expr_type::mul, Expression {2}, one_expr.clone()};
        EXPECT_EQ(two_mul_one, two_mul_a.diff("a"));
    }
    {
        const auto a_mul_two = Expression {binary_expr_type::mul, var_a.clone(), Expression {2}};
        EXPECT_EQ(zero_expr, a_mul_two.diff(""));
        EXPECT_EQ(zero_expr, a_mul_two.diff("1"));
        EXPECT_EQ(zero_expr, a_mul_two.diff("2"));
        EXPECT_EQ(zero_expr, a_mul_two.diff("b"));

        const auto one_mul_two = Expression {binary_expr_type::mul, one_expr.clone(), Expression {2}};
        EXPECT_EQ(one_mul_two, a_mul_two.diff("a"));
    }
    {
        const auto a_mul_b = Expression {binary_expr_type::mul, var_a.clone(), var_b.clone()};
        EXPECT_EQ(zero_expr, a_mul_b.diff(""));
        EXPECT_EQ(zero_expr, a_mul_b.diff("1"));

        const auto one_mul_b = Expression {binary_expr_type::mul, one_expr.clone(), var_b.clone()};
        EXPECT_EQ(one_mul_b, a_mul_b.diff("a"));

        const auto a_mul_one = Expression {binary_expr_type::mul, var_a.clone(), one_expr.clone()};
        EXPECT_EQ(a_mul_one, a_mul_b.diff("b"));
    }
    {
        const auto a_mul_a = Expression {binary_expr_type::mul, var_a.clone(), var_a.clone()};

        EXPECT_EQ(zero_expr, a_mul_a.diff(""));
        EXPECT_EQ(zero_expr, a_mul_a.diff("1"));
        EXPECT_EQ(zero_expr, a_mul_a.diff("b"));

        auto a_mul_one = Expression {binary_expr_type::mul, var_a.clone(), one_expr.clone()};
        auto one_mul_a = Expression {binary_expr_type::mul, one_expr.clone(), var_a.clone()};
        const auto summ = Expression {binary_expr_type::sum, std::move(one_mul_a), std::move(a_mul_one)};
        EXPECT_EQ(summ, a_mul_a.diff("a"));
    }
}

TEST_F(ExpressionTests, simple_div_tests) {
    {
        const auto two_div_three = Expression {binary_expr_type::div, Expression {2}, Expression {3}};

        EXPECT_EQ(zero_expr, two_div_three.diff(""));
        EXPECT_EQ(zero_expr, two_div_three.diff("1"));
        EXPECT_EQ(zero_expr, two_div_three.diff("a"));
        EXPECT_EQ(zero_expr, two_div_three.diff("b"));
    }
    {
        const auto a_div_two = Expression {binary_expr_type::div, var_a.clone(), Expression {2}};

        EXPECT_EQ(zero_expr, a_div_two.diff(""));
        EXPECT_EQ(zero_expr, a_div_two.diff("1"));
        EXPECT_EQ(zero_expr, a_div_two.diff("b"));

        auto one_div_two = Expression {binary_expr_type::div, one_expr.clone(), two_expr.clone()};
        const auto diff_result = Expression {binary_expr_type::mul, std::move(one_div_two), one_expr.clone()};
        EXPECT_EQ(diff_result, a_div_two.diff("a"));
    }
    {
        const auto two_div_a = Expression {binary_expr_type::div, Expression {2}, var_a.clone()};

        EXPECT_EQ(zero_expr, two_div_a.diff(""));
        EXPECT_EQ(zero_expr, two_div_a.diff("1"));
        EXPECT_EQ(zero_expr, two_div_a.diff("b"));

        auto a_squared = Expression {binary_expr_type::mul, var_a.clone(), var_a.clone()};
        auto one_div_a_squared = Expression {binary_expr_type::div, one_expr.clone(), std::move(a_squared)};
        auto two_mul_one_div_a_squared =
            Expression {binary_expr_type::mul, two_expr.clone(), std::move(one_div_a_squared)};
        const auto diff_result = Expression {unary_expr_type::unary_minus, std::move(two_mul_one_div_a_squared)};

        EXPECT_EQ(diff_result, two_div_a.diff("a"));
    }
}