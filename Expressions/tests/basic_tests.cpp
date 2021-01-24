#include "Expression.h"
#include "Parser.h"
#include "Variable.h"

#include "gtest/gtest.h"

TEST(BasicTests, CreateTest)
{
    auto var = parse("a");
    ASSERT_STREQ(var->tostring().c_str(), "a");
}
