#include "ParserInternal.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using namespace testing;
using namespace parser::internal;

TEST(LexerTest, VariableParsing) {
    EXPECT_THAT(splice_string("a"), ElementsAre("a"));
    EXPECT_THAT(splice_string("ab"), ElementsAre("ab"));
    EXPECT_THAT(splice_string("abc"), ElementsAre("abc"));
    EXPECT_THAT(splice_string("1abc"), ElementsAre("1abc"));
    EXPECT_THAT(splice_string("abc1"), ElementsAre("abc1"));
    EXPECT_THAT(splice_string("1abc1"), ElementsAre("1abc1"));

    EXPECT_THAT(splice_string("1"), ElementsAre("1"));
    EXPECT_THAT(splice_string("11"), ElementsAre("11"));
    EXPECT_THAT(splice_string("111"), ElementsAre("111"));
}

TEST(LexerTest, OperationParsing) {
    EXPECT_THAT(splice_string("+"), ElementsAre("+"));
    EXPECT_THAT(splice_string("-"), ElementsAre("-/u"));
    EXPECT_THAT(splice_string("/"), ElementsAre("/"));
    EXPECT_THAT(splice_string("*"), ElementsAre("*"));

    EXPECT_THAT(splice_string("++"), ElementsAre("+", "+"));
    EXPECT_THAT(splice_string("+-"), ElementsAre("+", "-/u"));
    EXPECT_THAT(splice_string("+/"), ElementsAre("+", "/"));
    EXPECT_THAT(splice_string("+*"), ElementsAre("+", "*"));
}

TEST(LexerTest, UnaryMinusParsing) {
    EXPECT_THAT(splice_string("-"), ElementsAre("-/u"));
    EXPECT_THAT(splice_string("--"), ElementsAre("-/u", "-/u"));
    EXPECT_THAT(splice_string("---"), ElementsAre("-/u", "-/u", "-/u"));

    EXPECT_THAT(splice_string("-a"), ElementsAre("-/u", "a"));
    EXPECT_THAT(splice_string("-1"), ElementsAre("-/u", "1"));
    EXPECT_THAT(splice_string("-aa"), ElementsAre("-/u", "aa"));
    EXPECT_THAT(splice_string("-a1"), ElementsAre("-/u", "a1"));
    EXPECT_THAT(splice_string("-1a"), ElementsAre("-/u", "1a"));

    EXPECT_THAT(splice_string("-("), ElementsAre("-/u", "("));
    EXPECT_THAT(splice_string("(-"), ElementsAre("(", "-/u"));
    EXPECT_THAT(splice_string("-)"), ElementsAre("-/u", ")"));
    EXPECT_THAT(splice_string(")-"), ElementsAre(")", "-"));

    EXPECT_THAT(splice_string("a-"), ElementsAre("a", "-"));
    EXPECT_THAT(splice_string("1-"), ElementsAre("1", "-"));
    EXPECT_THAT(splice_string("a1-"), ElementsAre("a1", "-"));
    EXPECT_THAT(splice_string("1a-"), ElementsAre("1a", "-"));
    EXPECT_THAT(splice_string("1a-"), ElementsAre("1a", "-"));
    EXPECT_THAT(splice_string("1a-"), ElementsAre("1a", "-"));
    EXPECT_THAT(splice_string("sin-"), ElementsAre("sin", "-"));
    EXPECT_THAT(splice_string("cos-"), ElementsAre("cos", "-"));
}

TEST(LexerTest, ExprParsing) {
    EXPECT_THAT(splice_string("a+a"), ElementsAre("a", "+", "a"));
    EXPECT_THAT(splice_string("a+1"), ElementsAre("a", "+", "1"));
    EXPECT_THAT(splice_string("1+a"), ElementsAre("1", "+", "a"));
    EXPECT_THAT(splice_string("1+1"), ElementsAre("1", "+", "1"));

    EXPECT_THAT(splice_string("a-a"), ElementsAre("a", "-", "a"));
    EXPECT_THAT(splice_string("a-1"), ElementsAre("a", "-", "1"));
    EXPECT_THAT(splice_string("1-a"), ElementsAre("1", "-", "a"));
    EXPECT_THAT(splice_string("1-1"), ElementsAre("1", "-", "1"));

    EXPECT_THAT(splice_string("a*a"), ElementsAre("a", "*", "a"));
    EXPECT_THAT(splice_string("a*1"), ElementsAre("a", "*", "1"));
    EXPECT_THAT(splice_string("1*a"), ElementsAre("1", "*", "a"));
    EXPECT_THAT(splice_string("1*1"), ElementsAre("1", "*", "1"));

    EXPECT_THAT(splice_string("a/a"), ElementsAre("a", "/", "a"));
    EXPECT_THAT(splice_string("a/1"), ElementsAre("a", "/", "1"));
    EXPECT_THAT(splice_string("1/a"), ElementsAre("1", "/", "a"));
    EXPECT_THAT(splice_string("1/1"), ElementsAre("1", "/", "1"));

    EXPECT_THAT(splice_string("(a+a)+a"), ElementsAre("(", "a", "+", "a", ")", "+", "a"));
    EXPECT_THAT(splice_string("a+(1)"), ElementsAre("a", "+", "(", "1", ")"));

    EXPECT_THAT(splice_string("sin(x)"), ElementsAre("sin", "(", "x", ")"));
    EXPECT_THAT(splice_string("cos(x)"), ElementsAre("cos", "(", "x", ")"));
    EXPECT_THAT(splice_string("sin(cos(x))"), ElementsAre("sin", "(", "cos", "(", "x", ")", ")"));
    EXPECT_THAT(splice_string("cos(sin(x))"), ElementsAre("cos", "(", "sin", "(", "x", ")", ")"));

    EXPECT_THAT(splice_string("sin(x+y)"), ElementsAre("sin", "(", "x", "+", "y", ")"));
    EXPECT_THAT(splice_string("cos(x+y)"), ElementsAre("cos", "(", "x", "+", "y", ")"));

    EXPECT_THAT(splice_string("-sin(x)"), ElementsAre("-/u", "sin", "(", "x", ")"));
    EXPECT_THAT(splice_string("-sin(-x)"), ElementsAre("-/u", "sin", "(", "-/u", "x", ")"));
    EXPECT_THAT(splice_string("-sin(-x)-"), ElementsAre("-/u", "sin", "(", "-/u", "x", ")", "-"));
}