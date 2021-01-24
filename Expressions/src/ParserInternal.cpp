#include "ParserInternal.h"

#include "Stack.h"

#include <iostream>
#include <unordered_map>

namespace parser::internal
{

inline namespace
{

TokenType token_operator_type(const std::string& token) {
    if (token == "+") return TokenType::sum;
    if (token == "-") return TokenType::sub;
    if (token == "/") return TokenType::div;
    if (token == "*") return TokenType::mul;
    if (token == "-/u") return TokenType::negate;
    if (token == "sin") return TokenType::sin;
    if (token == "cos") return TokenType::cos;

    return TokenType::err;
}

bool is_operator(const std::string& token) {
    return (token == "*" || token == "+" || token == "-" || token == "/" || token == "-/u");
}

bool is_function(const std::string& token) {
    return token == "sin" || token == "cos";
}

bool is_number(const std::string& token) {
    return std::all_of(token.begin(), token.end(), [](const auto& it) {
        return std::isdigit(it) || it == '.' || it == ',';
    });
}

bool is_variable(const std::string& token) {
    return std::all_of(token.begin(), token.end(), ::isalnum);
}

bool is_open_par(const std::string& token) {
    return token == "(";
}

bool is_close_par(const std::string& token) {
    return token == ")";
}

bool is_parenthesis(const std::string& token) {
    return token == "(" || token == ")";
}

} // namespace

std::vector<std::string> splice_to_tokens(const std::string& input) {
    constexpr auto delimiters = "+-*/()";

    std::vector<std::string> result;
    std::size_t prev = 0, pos;

    while ((pos = input.find_first_of(delimiters, prev)) != std::string::npos) {
        if (pos > prev) {
            result.emplace_back(input.substr(prev, pos - prev));
        }
        auto op_token = input.substr(pos, 1);
        // unary minus can be at the start of the expression so better check if result is empty
        if (op_token == "-" && (result.empty() || (!is_number(result.back()) && !is_variable(result.back())))) {
            op_token = std::string {"-/u"};
        }
        result.emplace_back(op_token);
        prev = pos + 1;
    }
    if (prev != input.length()) {
        result.emplace_back(input.substr(prev));
    }

    return result;
}

// also known as shunting yard algorithm
std::vector<ParsedToken> convert_to_reverse_notation(const std::vector<std::string>& tokens) {
    std::vector<ParsedToken> result;
    result.reserve(tokens.size());
    Stack<std::string> stack;

    const std::unordered_map<std::string, int> OPERATOR_PRIORITY {
        {"+", 1},
        {"-", 1},
        {"*", 2},
        {"/", 2},
        {"-/u", 3},
        {"cos", 3},
        {"sin", 3},
        {"(", 0},
        {")", 0},
    };

    for (const auto& token : tokens) {
        if (is_number(token)) {
            result.emplace_back(ParsedToken {TokenType::number, token});
        } else if (is_variable(token) && !is_function(token)) {
            result.emplace_back(ParsedToken {TokenType::variable, token});
        } else if (is_operator(token) || is_parenthesis(token) || is_function(token)) {
            if (!is_open_par(token)) {
                // pop stack until :
                // '(' met if current token is ')'
                // or if op_priority(token) >= op_priority(stack.top)
                while (!stack.empty()
                       && ((is_close_par(token) && !is_open_par(stack.top()))
                           || (OPERATOR_PRIORITY.at(stack.top()) > OPERATOR_PRIORITY.at(token))))
                {
                    auto stack_top = stack.pop();
                    result.emplace_back(ParsedToken {token_operator_type(stack_top), stack_top});
                }

                if (is_close_par(token) && !stack.empty()) {
                    stack.pop();
                }
            }

            if (!is_close_par(token)) {
                stack.push(token);
            }
        } else {
            std::cout << "Input error! Token: \"" << token << "\" not recognized. Skipping.\n";
        }
    }
    while (!stack.empty()) {
        auto token_from_stack = stack.pop();
        if (!is_parenthesis(token_from_stack)) {
            result.emplace_back(ParsedToken {token_operator_type(token_from_stack), token_from_stack});
        }
    }

    return result;
}
} // namespace parser::internal