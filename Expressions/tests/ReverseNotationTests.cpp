#include "ParserInternal.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using namespace testing;
using namespace parser::internal;

TEST(ReverseNotationTests, TokenRecognition) {
    EXPECT_THAT(convert_to_reverse_notation({"a"}), ElementsAre(ParsedToken {TokenType::variable, "a"}));
    EXPECT_THAT(convert_to_reverse_notation({"aa"}), ElementsAre(ParsedToken {TokenType::variable, "aa"}));
    EXPECT_THAT(convert_to_reverse_notation({"a1"}), ElementsAre(ParsedToken {TokenType::variable, "a1"}));
    EXPECT_THAT(convert_to_reverse_notation({"1a"}), ElementsAre(ParsedToken {TokenType::variable, "1a"}));
    EXPECT_THAT(convert_to_reverse_notation({"1"}), ElementsAre(ParsedToken {TokenType::number, "1"}));
    EXPECT_THAT(convert_to_reverse_notation({"11"}), ElementsAre(ParsedToken {TokenType::number, "11"}));

    EXPECT_THAT(convert_to_reverse_notation({"+"}), ElementsAre(ParsedToken {TokenType::sum, "+"}));
    EXPECT_THAT(convert_to_reverse_notation({"-"}), ElementsAre(ParsedToken {TokenType::sub, "-"}));
    EXPECT_THAT(convert_to_reverse_notation({"/"}), ElementsAre(ParsedToken {TokenType::div, "/"}));
    EXPECT_THAT(convert_to_reverse_notation({"*"}), ElementsAre(ParsedToken {TokenType::mul, "*"}));

    EXPECT_THAT(convert_to_reverse_notation({"-/u"}), ElementsAre(ParsedToken {TokenType::negate, "-/u"}));
    EXPECT_THAT(convert_to_reverse_notation({"sin"}), ElementsAre(ParsedToken {TokenType::sin, "sin"}));
    EXPECT_THAT(convert_to_reverse_notation({"cos"}), ElementsAre(ParsedToken {TokenType::cos, "cos"}));

    EXPECT_THAT(convert_to_reverse_notation({"("}), ElementsAre());
    EXPECT_THAT(convert_to_reverse_notation({")"}), ElementsAre());
}

TEST(ReverseNotationTests, BasicReverse) {
    const auto a_tok = ParsedToken {TokenType::variable, "a"};
    const auto b_tok = ParsedToken {TokenType::variable, "b"};
    const auto c_tok = ParsedToken {TokenType::variable, "c"};
    const auto num_tok = ParsedToken {TokenType::number, "1"};
    const auto sum_tok = ParsedToken {TokenType::sum, "+"};
    const auto sub_tok = ParsedToken {TokenType::sub, "-"};
    const auto mul_tok = ParsedToken {TokenType::mul, "*"};
    const auto div_tok = ParsedToken {TokenType::div, "/"};
    const auto neg_tok = ParsedToken {TokenType::div, "-/u"};

    EXPECT_THAT(convert_to_reverse_notation({"a"}), ElementsAre(a_tok));
    EXPECT_THAT(convert_to_reverse_notation({"a", "+", "b"}), ElementsAre(a_tok, b_tok, sum_tok));
    EXPECT_THAT(convert_to_reverse_notation({"a", "-", "b"}), ElementsAre(a_tok, b_tok, sub_tok));
    EXPECT_THAT(convert_to_reverse_notation({"a", "*", "b"}), ElementsAre(a_tok, b_tok, mul_tok));
    EXPECT_THAT(convert_to_reverse_notation({"a", "/", "b"}), ElementsAre(a_tok, b_tok, div_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({"a", "+", "b", "+", "c"}),
        ElementsAre(a_tok, b_tok, c_tok, sum_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"a", "+", "b", "-", "c"}),
        ElementsAre(a_tok, b_tok, c_tok, sub_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"a", "-", "b", "+", "c"}),
        ElementsAre(a_tok, b_tok, c_tok, sum_tok, sub_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"a", "-", "b", "-", "c"}),
        ElementsAre(a_tok, b_tok, c_tok, sub_tok, sub_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({"a", "*", "b", "+", "c"}),
        ElementsAre(a_tok, b_tok, mul_tok, c_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"a", "+", "b", "*", "c"}),
        ElementsAre(a_tok, b_tok, c_tok, mul_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"a", "*", "b", "*", "c"}),
        ElementsAre(a_tok, b_tok, c_tok, mul_tok, mul_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"a", "*", "b", "/", "c"}),
        ElementsAre(a_tok, b_tok, c_tok, div_tok, mul_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"a", "/", "b", "/", "c"}),
        ElementsAre(a_tok, b_tok, c_tok, div_tok, div_tok));
}

TEST(ReverseNotation, ParenthesisTest) {
    const auto a_tok = ParsedToken {TokenType::variable, "a"};
    const auto b_tok = ParsedToken {TokenType::variable, "b"};
    const auto c_tok = ParsedToken {TokenType::variable, "c"};
    const auto num_tok = ParsedToken {TokenType::number, "1"};
    const auto sum_tok = ParsedToken {TokenType::sum, "+"};
    const auto sub_tok = ParsedToken {TokenType::sub, "-"};
    const auto mul_tok = ParsedToken {TokenType::mul, "*"};
    const auto div_tok = ParsedToken {TokenType::div, "/"};
    const auto neg_tok = ParsedToken {TokenType::div, "-/u"};

    EXPECT_THAT(convert_to_reverse_notation({"(", "a", ")"}), ElementsAre(a_tok));
    EXPECT_THAT(convert_to_reverse_notation({"(", "(", "a", ")", ")"}), ElementsAre(a_tok));
    EXPECT_THAT(convert_to_reverse_notation({"(", "1", ")"}), ElementsAre(num_tok));

    EXPECT_THAT(convert_to_reverse_notation({"(", "a", "+", "b", ")"}), ElementsAre(a_tok, b_tok, sum_tok));
    EXPECT_THAT(convert_to_reverse_notation({"(", "a", "-", "b", ")"}), ElementsAre(a_tok, b_tok, sub_tok));
    EXPECT_THAT(convert_to_reverse_notation({"(", "a", "*", "b", ")"}), ElementsAre(a_tok, b_tok, mul_tok));
    EXPECT_THAT(convert_to_reverse_notation({"(", "a", "/", "b", ")"}), ElementsAre(a_tok, b_tok, div_tok));

    EXPECT_THAT(convert_to_reverse_notation({"(", "a", ")", "+", "(", "b", ")"}), ElementsAre(a_tok, b_tok, sum_tok));

    EXPECT_THAT(convert_to_reverse_notation({"(", "a", "/", "b", ")"}), ElementsAre(a_tok, b_tok, div_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({"(", "a", "+", "b", ")", "+", "c"}),
        ElementsAre(a_tok, b_tok, sum_tok, c_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"(", "a", "+", "b", ")", "/", "c"}),
        ElementsAre(a_tok, b_tok, sum_tok, c_tok, div_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"(", "a", "*", "b", ")", "/", "c"}),
        ElementsAre(a_tok, b_tok, mul_tok, c_tok, div_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"a", "*", "(", "b", "/", "c", ")"}),
        ElementsAre(a_tok, b_tok, c_tok, div_tok, mul_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({"a", "+", "(", "b", "+", "c", ")"}),
        ElementsAre(a_tok, b_tok, c_tok, sum_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"a", "+", "(", "b", "-", "c", ")"}),
        ElementsAre(a_tok, b_tok, c_tok, sub_tok, sum_tok));

    EXPECT_THAT(
        convert_to_reverse_notation({"(", "a", "+", "(", "b", "+", "c", ")", ")"}),
        ElementsAre(a_tok, b_tok, c_tok, sum_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"(", "(", "a", ")", "+", "(", "b", "+", "c", ")", ")"}),
        ElementsAre(a_tok, b_tok, c_tok, sum_tok, sum_tok));
    EXPECT_THAT(
        convert_to_reverse_notation({"(", "(", "a", ")", "+", "(", "(", "b", ")", "+", "(", "c", ")", ")", ")"}),
        ElementsAre(a_tok, b_tok, c_tok, sum_tok, sum_tok));
}