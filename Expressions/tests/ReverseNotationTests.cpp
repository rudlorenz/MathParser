#include <ParserInternal.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using namespace testing;
using namespace parser::internal;

TEST(ReverseNotationTests, TokenRecognition) {
    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::variable, "a"}}),
        ElementsAre(ParsedToken {TokenType::variable, "a"}));
    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::variable, "aa"}}),
        ElementsAre(ParsedToken {TokenType::variable, "aa"}));
    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::variable, "a1"}}),
        ElementsAre(ParsedToken {TokenType::variable, "a1"}));
    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::variable, "1a"}}),
        ElementsAre(ParsedToken {TokenType::variable, "1a"}));
    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::number, "1"}}),
        ElementsAre(ParsedToken {TokenType::number, "1"}));
    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::number, "11"}}),
        ElementsAre(ParsedToken {TokenType::number, "11"}));

    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::sum, std::nullopt}}),
        ElementsAre(ParsedToken {TokenType::sum, std::nullopt}));
    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::sub, std::nullopt}}),
        ElementsAre(ParsedToken {TokenType::sub, std::nullopt}));
    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::div, std::nullopt}}),
        ElementsAre(ParsedToken {TokenType::div, std::nullopt}));
    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::mul, std::nullopt}}),
        ElementsAre(ParsedToken {TokenType::mul, std::nullopt}));

    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::negate, std::nullopt}}),
        ElementsAre(ParsedToken {TokenType::negate, std::nullopt}));
    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::sin, std::nullopt}}),
        ElementsAre(ParsedToken {TokenType::sin, std::nullopt}));
    EXPECT_THAT(
        convert_to_reverse_notation({ParsedToken {TokenType::cos, std::nullopt}}),
        ElementsAre(ParsedToken {TokenType::cos, std::nullopt}));

    EXPECT_THAT(convert_to_reverse_notation({ParsedToken {TokenType::open_par, std::nullopt}}), ElementsAre());
    EXPECT_THAT(convert_to_reverse_notation({ParsedToken {TokenType::close_par, std::nullopt}}), ElementsAre());
}

TEST(ReverseNotationTests, BasicReverse) {
    const auto a_tok = ParsedToken {TokenType::variable, "a"};
    const auto b_tok = ParsedToken {TokenType::variable, "b"};
    const auto c_tok = ParsedToken {TokenType::variable, "c"};
    const auto num_tok = ParsedToken {TokenType::number, "1"};
    const auto sum_tok = ParsedToken {TokenType::sum, std::nullopt};
    const auto sub_tok = ParsedToken {TokenType::sub, std::nullopt};
    const auto mul_tok = ParsedToken {TokenType::mul, std::nullopt};
    const auto div_tok = ParsedToken {TokenType::div, std::nullopt};
    const auto neg_tok = ParsedToken {TokenType::negate, std::nullopt};

    EXPECT_THAT(convert_to_reverse_notation({a_tok}), ElementsAre(a_tok));
    EXPECT_THAT(convert_to_reverse_notation({a_tok, sum_tok, b_tok}), ElementsAre(a_tok, b_tok, sum_tok));
    EXPECT_THAT(convert_to_reverse_notation({a_tok, sub_tok, b_tok}), ElementsAre(a_tok, b_tok, sub_tok));
    EXPECT_THAT(convert_to_reverse_notation({a_tok, mul_tok, b_tok}), ElementsAre(a_tok, b_tok, mul_tok));
    EXPECT_THAT(convert_to_reverse_notation({a_tok, div_tok, b_tok}), ElementsAre(a_tok, b_tok, div_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, sum_tok, b_tok, sum_tok, c_tok}),
        ElementsAre(a_tok, b_tok, sum_tok, c_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, sum_tok, b_tok, sub_tok, c_tok}),
        ElementsAre(a_tok, b_tok, sum_tok, c_tok, sub_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, sub_tok, b_tok, sum_tok, c_tok}),
        ElementsAre(a_tok, b_tok, sub_tok, c_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, sub_tok, b_tok, sub_tok, c_tok}),
        ElementsAre(a_tok, b_tok, sub_tok, c_tok, sub_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, mul_tok, b_tok, sum_tok, c_tok}),
        ElementsAre(a_tok, b_tok, mul_tok, c_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, sum_tok, b_tok, mul_tok, c_tok}),
        ElementsAre(a_tok, b_tok, sum_tok, c_tok, mul_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, mul_tok, b_tok, mul_tok, c_tok}),
        ElementsAre(a_tok, b_tok, mul_tok, c_tok, mul_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, mul_tok, b_tok, div_tok, c_tok}),
        ElementsAre(a_tok, b_tok, mul_tok, c_tok, div_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, div_tok, b_tok, div_tok, c_tok}),
        ElementsAre(a_tok, b_tok, div_tok, c_tok, div_tok));
}

TEST(ReverseNotation, ParenthesisTest) {
    const auto a_tok = ParsedToken {TokenType::variable, "a"};
    const auto b_tok = ParsedToken {TokenType::variable, "b"};
    const auto c_tok = ParsedToken {TokenType::variable, "c"};
    const auto num_tok = ParsedToken {TokenType::number, "1"};
    const auto sum_tok = ParsedToken {TokenType::sum, std::nullopt};
    const auto sub_tok = ParsedToken {TokenType::sub, std::nullopt};
    const auto mul_tok = ParsedToken {TokenType::mul, std::nullopt};
    const auto div_tok = ParsedToken {TokenType::div, std::nullopt};
    const auto neg_tok = ParsedToken {TokenType::negate, std::nullopt};
    const auto open_par = ParsedToken {TokenType::open_par, std::nullopt};
    const auto close_par = ParsedToken {TokenType::close_par, std::nullopt};

    EXPECT_THAT(convert_to_reverse_notation({open_par, a_tok, close_par}), ElementsAre(a_tok));
    EXPECT_THAT(convert_to_reverse_notation({open_par, open_par, a_tok, close_par, close_par}), ElementsAre(a_tok));
    EXPECT_THAT(convert_to_reverse_notation({open_par, num_tok, close_par}), ElementsAre(num_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({open_par, a_tok, sum_tok, b_tok, close_par}),
        ElementsAre(a_tok, b_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({open_par, a_tok, sub_tok, b_tok, close_par}),
        ElementsAre(a_tok, b_tok, sub_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({open_par, a_tok, mul_tok, b_tok, close_par}),
        ElementsAre(a_tok, b_tok, mul_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({open_par, a_tok, div_tok, b_tok, close_par}),
        ElementsAre(a_tok, b_tok, div_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({open_par, a_tok, close_par, sum_tok, open_par, b_tok, close_par}),
        ElementsAre(a_tok, b_tok, sum_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({open_par, a_tok, div_tok, b_tok, close_par}),
        ElementsAre(a_tok, b_tok, div_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({open_par, a_tok, sum_tok, b_tok, close_par, sum_tok, c_tok}),
        ElementsAre(a_tok, b_tok, sum_tok, c_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({open_par, a_tok, sum_tok, b_tok, close_par, div_tok, c_tok}),
        ElementsAre(a_tok, b_tok, sum_tok, c_tok, div_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({open_par, a_tok, mul_tok, b_tok, close_par, div_tok, c_tok}),
        ElementsAre(a_tok, b_tok, mul_tok, c_tok, div_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, mul_tok, open_par, b_tok, div_tok, c_tok, close_par}),
        ElementsAre(a_tok, b_tok, c_tok, div_tok, mul_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, sum_tok, open_par, b_tok, sum_tok, c_tok, close_par}),
        ElementsAre(a_tok, b_tok, c_tok, sum_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({a_tok, sum_tok, open_par, b_tok, sub_tok, c_tok, close_par}),
        ElementsAre(a_tok, b_tok, c_tok, sub_tok, sum_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({open_par, a_tok, sum_tok, open_par, b_tok, sum_tok, c_tok, close_par, close_par}),
        ElementsAre(a_tok, b_tok, c_tok, sum_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation(
            {open_par, open_par, a_tok, close_par, sum_tok, open_par, b_tok, sum_tok, c_tok, close_par, close_par}),
        ElementsAre(a_tok, b_tok, c_tok, sum_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation(
            {open_par,
             open_par,
             a_tok,
             close_par,
             sum_tok,
             open_par,
             open_par,
             b_tok,
             close_par,
             sum_tok,
             open_par,
             c_tok,
             close_par,
             close_par,
             close_par}),
        ElementsAre(a_tok, b_tok, c_tok, sum_tok, sum_tok));
}