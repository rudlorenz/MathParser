#pragma once

#include "Expressions.h"
#include "Stack.h"

#include <optional>
#include <string>
#include <unordered_map>
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
    open_par,
    close_par,
    err,
};

struct ParsedToken
{
public:
    int priority() const;
    std::string to_string() const;
    bool operator==(const ParsedToken& p) const = default;
    friend std::ostream& operator<<(std::ostream& os, const ParsedToken& tok);

public:
    TokenType type;
    std::optional<std::string> value;
};

std::vector<std::string> splice_string(const std::string& input);
std::vector<ParsedToken> convert_to_tokens(const std::vector<std::string>& spliced);
std::vector<ParsedToken> convert_to_reverse_notation(const std::vector<ParsedToken>& tokens);
std::shared_ptr<Expression> reverse_notation_to_expression(const std::vector<ParsedToken>& parsed_tokens);

} // namespace parser::internal