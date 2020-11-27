#include "gtest/gtest.h"

#include "Expression.h"
#include "Variable.h"
#include "Parser.h"

TEST(BasicTests, CreateTest)
{
    auto var = parse("a");
    ASSERT_STREQ(var->tostring().c_str(), "a");
}
