#include "ParserInternal.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using namespace testing;
using namespace parser::internal;

TEST(LexerTest, VariableParsing) {
    EXPECT_THAT(splice_to_tokens("a"), ElementsAre("a"));
    EXPECT_THAT(splice_to_tokens("ab"), ElementsAre("ab"));
    EXPECT_THAT(splice_to_tokens("abc"), ElementsAre("abc"));
    EXPECT_THAT(splice_to_tokens("1abc"), ElementsAre("1abc"));
    EXPECT_THAT(splice_to_tokens("abc1"), ElementsAre("abc1"));
    EXPECT_THAT(splice_to_tokens("1abc1"), ElementsAre("1abc1"));

    EXPECT_THAT(splice_to_tokens("1"), ElementsAre("1"));
    EXPECT_THAT(splice_to_tokens("11"), ElementsAre("11"));
    EXPECT_THAT(splice_to_tokens("111"), ElementsAre("111"));
}

TEST(LexerTest, OperationParsing) {
    EXPECT_THAT(splice_to_tokens("+"), ElementsAre("+"));
    EXPECT_THAT(splice_to_tokens("-"), ElementsAre("-/u"));
    EXPECT_THAT(splice_to_tokens("/"), ElementsAre("/"));
    EXPECT_THAT(splice_to_tokens("*"), ElementsAre("*"));

    EXPECT_THAT(splice_to_tokens("++"), ElementsAre("+", "+"));
    EXPECT_THAT(splice_to_tokens("+-"), ElementsAre("+", "-/u"));
    EXPECT_THAT(splice_to_tokens("+/"), ElementsAre("+", "/"));
    EXPECT_THAT(splice_to_tokens("+*"), ElementsAre("+", "*"));
}

TEST(LexerTest, UnaryMinusParsing) {
    EXPECT_THAT(splice_to_tokens("-"), ElementsAre("-/u"));
    EXPECT_THAT(splice_to_tokens("--"), ElementsAre("-/u", "-/u"));
    EXPECT_THAT(splice_to_tokens("---"), ElementsAre("-/u", "-/u", "-/u"));

    EXPECT_THAT(splice_to_tokens("-a"), ElementsAre("-/u", "a"));
    EXPECT_THAT(splice_to_tokens("-1"), ElementsAre("-/u", "1"));
    EXPECT_THAT(splice_to_tokens("-aa"), ElementsAre("-/u", "aa"));
    EXPECT_THAT(splice_to_tokens("-a1"), ElementsAre("-/u", "a1"));
    EXPECT_THAT(splice_to_tokens("-1a"), ElementsAre("-/u", "1a"));

    EXPECT_THAT(splice_to_tokens("-("), ElementsAre("-/u", "("));
    EXPECT_THAT(splice_to_tokens("(-"), ElementsAre("(", "-/u"));
    EXPECT_THAT(splice_to_tokens("-)"), ElementsAre("-/u", ")"));
    EXPECT_THAT(splice_to_tokens(")-"), ElementsAre(")", "-"));

    EXPECT_THAT(splice_to_tokens("a-"), ElementsAre("a", "-"));
    EXPECT_THAT(splice_to_tokens("1-"), ElementsAre("1", "-"));
    EXPECT_THAT(splice_to_tokens("a1-"), ElementsAre("a1", "-"));
    EXPECT_THAT(splice_to_tokens("1a-"), ElementsAre("1a", "-"));
    EXPECT_THAT(splice_to_tokens("1a-"), ElementsAre("1a", "-"));
    EXPECT_THAT(splice_to_tokens("1a-"), ElementsAre("1a", "-"));
    EXPECT_THAT(splice_to_tokens("sin-"), ElementsAre("sin", "-"));
    EXPECT_THAT(splice_to_tokens("cos-"), ElementsAre("cos", "-"));
}

TEST(LexerTest, ExprParsing) {
    EXPECT_THAT(splice_to_tokens("a+a"), ElementsAre("a", "+", "a"));
    EXPECT_THAT(splice_to_tokens("a+1"), ElementsAre("a", "+", "1"));
    EXPECT_THAT(splice_to_tokens("1+a"), ElementsAre("1", "+", "a"));
    EXPECT_THAT(splice_to_tokens("1+1"), ElementsAre("1", "+", "1"));

    EXPECT_THAT(splice_to_tokens("a-a"), ElementsAre("a", "-", "a"));
    EXPECT_THAT(splice_to_tokens("a-1"), ElementsAre("a", "-", "1"));
    EXPECT_THAT(splice_to_tokens("1-a"), ElementsAre("1", "-", "a"));
    EXPECT_THAT(splice_to_tokens("1-1"), ElementsAre("1", "-", "1"));

    EXPECT_THAT(splice_to_tokens("a*a"), ElementsAre("a", "*", "a"));
    EXPECT_THAT(splice_to_tokens("a*1"), ElementsAre("a", "*", "1"));
    EXPECT_THAT(splice_to_tokens("1*a"), ElementsAre("1", "*", "a"));
    EXPECT_THAT(splice_to_tokens("1*1"), ElementsAre("1", "*", "1"));

    EXPECT_THAT(splice_to_tokens("a/a"), ElementsAre("a", "/", "a"));
    EXPECT_THAT(splice_to_tokens("a/1"), ElementsAre("a", "/", "1"));
    EXPECT_THAT(splice_to_tokens("1/a"), ElementsAre("1", "/", "a"));
    EXPECT_THAT(splice_to_tokens("1/1"), ElementsAre("1", "/", "1"));

    EXPECT_THAT(splice_to_tokens("(a+a)+a"), ElementsAre("(", "a", "+", "a", ")", "+", "a"));
    EXPECT_THAT(splice_to_tokens("a+(1)"), ElementsAre("a", "+", "(", "1", ")"));

    EXPECT_THAT(splice_to_tokens("sin(x)"), ElementsAre("sin", "(", "x", ")"));
    EXPECT_THAT(splice_to_tokens("cos(x)"), ElementsAre("cos", "(", "x", ")"));
    EXPECT_THAT(splice_to_tokens("sin(cos(x))"), ElementsAre("sin", "(", "cos", "(", "x", ")", ")"));
    EXPECT_THAT(splice_to_tokens("cos(sin(x))"), ElementsAre("cos", "(", "sin", "(", "x", ")", ")"));

    EXPECT_THAT(splice_to_tokens("sin(x+y)"), ElementsAre("sin", "(", "x", "+", "y", ")"));
    EXPECT_THAT(splice_to_tokens("cos(x+y)"), ElementsAre("cos", "(", "x", "+", "y", ")"));

    EXPECT_THAT(splice_to_tokens("-sin(x)"), ElementsAre("-/u", "sin", "(", "x", ")"));
    EXPECT_THAT(splice_to_tokens("-sin(-x)"), ElementsAre("-/u", "sin", "(", "-/u", "x", ")"));
    EXPECT_THAT(splice_to_tokens("-sin(-x)-"), ElementsAre("-/u", "sin", "(", "-/u", "x", ")", "-"));
}