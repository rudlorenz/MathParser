#include "Parser.h"

#include "Sum.h"
#include "Sub.h"
#include "Mul.h"
#include "Div.h"
#include "Number.h"
#include "Variable.h"

#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <iostream>

inline namespace
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
    err,
};

struct ParsedToken
{
    TokenType type;
    std::string value;
};

template<typename T>
struct Stack : public std::vector<T>
{
    using base_type = std::vector<T>;
    void push(const T& token) { base_type::emplace_back(token); };
    const T& top() const { return base_type::back(); };

    T pop()
    {
        auto result = base_type::back();
        base_type::pop_back();
        return result;
    }
};

TokenType token_operator_type(const std::string& token)
{
    if (token == "+") return TokenType::sum;
    if (token == "-") return TokenType::sub;
    if (token == "/") return TokenType::div;
    if (token == "*") return TokenType::mul;

    return TokenType::err;
}

std::string toktostring(const ParsedToken& token) {
    switch (token.type)
    {
    case TokenType::sum: return std::string("+");
    case TokenType::sub: return std::string("-");
    case TokenType::div: return std::string("/");
    case TokenType::mul: return std::string("*");
    case TokenType::sin: return std::string("sin");
    case TokenType::cos: return std::string("cos");
    case TokenType::number: return std::string("num ") + token.value;
    case TokenType::variable: return std::string("var ") + token.value;
    default: return std::string("error");
    }
}

std::shared_ptr<Expression> token_to_expression(const ParsedToken& token)
{
    switch (token.type)
    {
    case TokenType::number:   return std::make_shared<Number>(std::stoi(token.value));
    case TokenType::variable: return std::make_shared<Variable>(token.value);
    default: return nullptr;
    }
}

std::shared_ptr<Expression> token_to_expression(
    const ParsedToken& token,
    const std::shared_ptr<Expression>& lhs,
    const std::shared_ptr<Expression>& rhs)
{
    switch (token.type)
    {
    case TokenType::sum: return std::make_shared<Sum>(lhs, rhs);
    case TokenType::sub: return std::make_shared<Sub>(lhs, rhs);
    case TokenType::div: return std::make_shared<Div>(lhs, rhs);
    case TokenType::mul: return std::make_shared<Mul>(lhs, rhs);
    default: return nullptr;
    }
}

bool is_operator(const std::string& token)
{
    return (token == "*" || token == "+" || token == "-" || token == "/");
}

bool is_operator(TokenType type)
{
    switch (type)
    {
    case TokenType::sum:
    case TokenType::sub:
    case TokenType::div:
    case TokenType::mul:
        return true;
    default:
        return false;
    }
}

bool is_number(const std::string& token)
{
    return std::all_of(
        token.begin(),
        token.end(),
        [] (const auto& it) { return std::isdigit(it) || it == '.' || it == ','; });
}

bool is_variable(const std::string& token)
{
    return std::all_of(token.begin(), token.end(), ::isalpha);
}

bool is_open_par(const std::string& token)
{
    return token == "(";
}

bool is_close_par(const std::string& token)
{
    return token == ")";
}

bool is_parenthesis(const std::string& token)
{
    return token == "(" || token == ")";
}

std::vector<std::string> splice_to_tokens(const std::string& input)
{
    const auto delimiters = "+-*/()";

    std::vector<std::string> result;
    std::size_t prev = 0, pos;

    while ((pos = input.find_first_of(delimiters, prev)) != std::string::npos)
    {
        if (pos > prev)
        {
            auto token = input.substr(prev, pos - prev);
            token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
            result.emplace_back(token);
        }
        result.emplace_back(input.substr(pos, 1));
        prev = pos + 1;
    }
    if (prev != std::string::npos)
    {
        result.emplace_back(input.substr(prev, std::string::npos));
    }

    return result;
}

std::vector<ParsedToken> convert_to_reverse_notation(const std::vector<std::string>& tokens)
{
    std::vector<ParsedToken> result;
    result.reserve(tokens.size());
    Stack<std::string> stack;

    const std::unordered_map<std::string, int> OPERATOR_PRIORITY{
        { "+", 1 },
        { "-", 1 },
        { "*", 2 },
        { "/", 2 },
        { "(", 0 },
        { ")", 0 },
    };

    for (const auto& token : tokens)
    {
        if (is_number(token)) {
            result.emplace_back(ParsedToken{ TokenType::number, token });
        }
        else if (is_variable(token)) {
            result.emplace_back(ParsedToken{ TokenType::variable, token });
        }
        else if (is_operator(token) || is_parenthesis(token))
        {
            if (!is_open_par(token))
            {
                // pop stack until :
                // '(' met if current token is ')'
                // or if op_priority(token) >= op_priority(stack.top)
                while (!stack.empty()
                    && ( (is_close_par(token) && !is_open_par(stack.top()) )
                        || (OPERATOR_PRIORITY.at(stack.top()) > OPERATOR_PRIORITY.at(token))))
                {
                    auto stack_top = stack.pop();
                    result.emplace_back(ParsedToken{ token_operator_type(stack_top), stack_top });
                }

                if (is_close_par(token) ) {
                    stack.pop();
                }
            }

            if (!is_close_par(token)) {
                stack.push(token);
            }
        }
        else {
            std::cout << "Input error! Token: \"" << token << "\" not recognized. Skipping.\n";
        }
    }
    while (!stack.empty())
    {
        auto token_from_stack = stack.pop();
        if (!is_parenthesis(token_from_stack)) {
            result.emplace_back(ParsedToken{ token_operator_type(token_from_stack), token_from_stack} );
        }
    }

    return result;
}


} // inline namespace


std::shared_ptr<Expression> parse(std::string input)
{
    input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());

    //!todo check for balanced brackets
    //!todo sin and cos parsing
    auto tokens = convert_to_reverse_notation(splice_to_tokens(input));

    Stack<std::shared_ptr<Expression>> temp;
    temp.reserve(tokens.size());

    for (auto& token : tokens)
    {
        if (is_operator(token.type) && temp.size() >= 2)
        {
            auto rhs = temp.pop();
            auto lhs = temp.pop();
            temp.push(token_to_expression(token, lhs, rhs));
        }
        else {
            temp.push(token_to_expression(token));
        }
    }

    return temp.pop();
}
