#pragma once

#include <string>
#include <vector>

namespace parser::internal
{

enum class TokenType
{
    sum,
    sub,
    div,
    mul,
    sin,
    cos,
    number,
    variable,
    negate,
    err,
};

struct ParsedToken
{
    TokenType type;
    std::string value;
};

std::vector<std::string> splice_to_tokens(const std::string& input);
std::vector<ParsedToken> convert_to_reverse_notation(const std::vector<std::string>& tokens);

} // namespace parser::internal