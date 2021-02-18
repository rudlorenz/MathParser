#include <ParserInternal.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using namespace testing;
using namespace parser::internal;

class ReverseNotationTests : public ::testing::Test
{
protected:
    void SetUp() override {
        a_tok = ParsedToken {TokenType::variable, "a"};
        b_tok = ParsedToken {TokenType::variable, "b"};
        c_tok = ParsedToken {TokenType::variable, "c"};
        num_tok = ParsedToken {TokenType::number, "1"};
        sum_tok = ParsedToken {TokenType::sum, std::nullopt};
        sub_tok = ParsedToken {TokenType::sub, std::nullopt};
        mul_tok = ParsedToken {TokenType::mul, std::nullopt};
        div_tok = ParsedToken {TokenType::div, std::nullopt};
        neg_tok = ParsedToken {TokenType::negate, std::nullopt};
        open_par = ParsedToken {TokenType::open_par, std::nullopt};
        close_par = ParsedToken {TokenType::close_par, std::nullopt};
    }

protected:
    ParsedToken a_tok;
    ParsedToken b_tok;
    ParsedToken c_tok;
    ParsedToken num_tok;
    ParsedToken sum_tok;
    ParsedToken sub_tok;
    ParsedToken mul_tok;
    ParsedToken div_tok;
    ParsedToken neg_tok;
    ParsedToken open_par;
    ParsedToken close_par;
};


TEST_F(ReverseNotationTests, TokenRecognition) {
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

    EXPECT_THAT(convert_to_reverse_notation({ParsedToken {TokenType::open_par, std::nullopt}}), ElementsAre());
    EXPECT_THAT(convert_to_reverse_notation({ParsedToken {TokenType::close_par, std::nullopt}}), ElementsAre());
}

TEST_F(ReverseNotationTests, BasicReverse) {
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

TEST_F(ReverseNotationTests, ParenthesisTest) {
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