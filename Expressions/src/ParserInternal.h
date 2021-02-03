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

class Recognizer
{
public:
    std::vector<ParsedToken> recognize_and_convert_to_infix(const std::vector<ParsedToken>& tokens);

private:
    bool process_operand(auto& token_stream_pos, auto token_stream_end);
    bool process_operator(auto& token_stream_pos, auto token_stream_end);

private:
    std::vector<ParsedToken> parsing_result;
    Stack<ParsedToken> stack;
};

std::vector<std::string> splice_string(const std::string& input);
std::vector<ParsedToken> convert_to_tokens(const std::vector<std::string>& spliced);
std::vector<ParsedToken> convert_to_reverse_notation(const std::vector<ParsedToken>& tokens);
std::shared_ptr<Expression> reverse_notation_to_expression(const std::vector<ParsedToken>& parsed_tokens);

} // namespace parser::internal