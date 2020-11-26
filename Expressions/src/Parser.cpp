#include "Parser.h"

#include "SinCos.h"
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
    negate,
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
    if (token == "-/u") return TokenType::negate;
    if (token == "sin") return TokenType::sin;
    if (token == "cos") return TokenType::cos;

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
    case TokenType::negate: return std::string("neg");
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

std::shared_ptr<Expression> function_token_to_expression(TokenType token_type, std::shared_ptr<Expression> param)
{
    switch (token_type)
    {
    case TokenType::negate: return std::make_shared<Negate>(std::move(param));
    case TokenType::sin:    return std::make_shared<Sin>(std::move(param));
    case TokenType::cos:    return std::make_shared<Cos>(std::move(param));
    default: return nullptr;
    }
}

std::shared_ptr<Expression> token_to_expression(
    TokenType token_type,
    const std::shared_ptr<Expression>& lhs,
    const std::shared_ptr<Expression>& rhs)
{
    switch (token_type)
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
    return (token == "*" || token == "+" || token == "-" || token == "/" || token == "-/u");
}

bool is_function(const std::string& token)
{
    return token == "sin" || token == "cos";
}

bool is_operator(TokenType type)
{
    switch (type)
    {
    case TokenType::sum:
    case TokenType::sub:
    case TokenType::div:
    case TokenType::mul:
    case TokenType::negate:
        return true;
    default:
        return false;
    }
}

bool is_unary_or_function(TokenType type)
{
    switch (type)
    {
    case TokenType::negate:
    case TokenType::sin:
    case TokenType::cos:
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
    constexpr auto delimiters = "+-*/()";

    std::vector<std::string> result;
    std::size_t prev = 0, pos;

    while ((pos = input.find_first_of(delimiters, prev)) != std::string::npos)
    {
        if (pos > prev) {
            result.emplace_back(input.substr(prev, pos - prev));
        }
        auto op_token = input.substr(pos, 1);
        // unary minus can be at the start of the expression so better check if result is empty
        if (op_token == "-" && (result.empty() || (!is_number(result.back()) && !is_variable(result.back())))) {
            op_token = std::string{"-/u"};
        }
        result.emplace_back(op_token);
        prev = pos + 1;
    }
    if (prev != input.length()) {
        result.emplace_back(input.substr(prev));
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
        {"-/u", 3},
        {"cos", 3},
        {"sin", 3},
        { "(", 0 },
        { ")", 0 },
    };

    for (const auto& token : tokens)
    {
        if (is_number(token)) {
            result.emplace_back(ParsedToken{ TokenType::number, token });
        }
        else if (is_variable(token) && !is_function(token)) {
            result.emplace_back(ParsedToken{ TokenType::variable, token });
        }
        else if (is_operator(token) || is_parenthesis(token) || is_function(token))
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

                if (is_close_par(token) && !stack.empty()) {
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
    auto tokens = convert_to_reverse_notation(splice_to_tokens(input));

    Stack<std::shared_ptr<Expression>> temp;
    temp.reserve(tokens.size());

    for (auto& token : tokens)
    {
        if (is_unary_or_function(token.type)) {
            temp.push(function_token_to_expression(token.type, temp.pop()));
        }
        else if (is_operator(token.type) && temp.size() >= 2)
        {
            auto rhs = temp.pop();
            auto lhs = temp.pop();
            temp.push(token_to_expression(token.type, lhs, rhs));
        }
        else {
            temp.push(token_to_expression(token));
        }
    }

    return temp.pop();
}
