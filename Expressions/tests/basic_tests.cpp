#include "Expression.h"
#include "Parser.h"
#include "Variable.h"

#include "gtest/gtest.h"

TEST(BasicTests, CreateTest) {
    auto var = parser::parse("a");
    ASSERT_STREQ(var->to_string().c_str(), "a");
}
