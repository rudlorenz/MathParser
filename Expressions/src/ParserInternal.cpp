#include "ParserInternal.h"

#include "Stack.h"

#include <fmt/core.h>

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

bool is_operator(const ParsedToken& token) {
    return token.type == TokenType::sum || token.type == TokenType::sub || token.type == TokenType::mul
        || token.type == TokenType::div || token.type == TokenType::negate;
}

bool is_function(const std::string& token) {
    return token == "sin" || token == "cos";
}

bool is_function(const ParsedToken& token) {
    return token.type == TokenType::sin || token.type == TokenType::cos;
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

bool is_parenthesis(const ParsedToken& token) {
    return token.type == TokenType::open_par || token.type == TokenType::close_par;
}

std::shared_ptr<Expression> token_to_expression(const ParsedToken& token) {
    switch (token.type) {
        case TokenType::number: return std::make_shared<Number>(std::stoi(token.value.value()));
        case TokenType::variable: return std::make_shared<Variable>(token.value.value());
        default: return nullptr;
    }
}

std::shared_ptr<Expression> function_token_to_expression(TokenType token_type, std::shared_ptr<Expression> param) {
    switch (token_type) {
        case TokenType::negate: return std::make_shared<Negate>(std::move(param));
        case TokenType::sin: return std::make_shared<Sin>(std::move(param));
        case TokenType::cos: return std::make_shared<Cos>(std::move(param));
        default: return nullptr;
    }
}

std::shared_ptr<Expression> token_to_expression(
    TokenType token_type,
    const std::shared_ptr<Expression>& lhs,
    const std::shared_ptr<Expression>& rhs) {
    switch (token_type) {
        case TokenType::sum: return std::make_shared<Sum>(lhs, rhs);
        case TokenType::sub: return std::make_shared<Sub>(lhs, rhs);
        case TokenType::div: return std::make_shared<Div>(lhs, rhs);
        case TokenType::mul: return std::make_shared<Mul>(lhs, rhs);
        default: return nullptr;
    }
}

bool is_operator(TokenType type) {
    switch (type) {
        case TokenType::sum:
        case TokenType::sub:
        case TokenType::div:
        case TokenType::mul:
        case TokenType::negate: return true;
        default: return false;
    }
}

bool is_unary_or_function(TokenType type) {
    switch (type) {
        case TokenType::negate:
        case TokenType::sin:
        case TokenType::cos: return true;
        default: return false;
    }
}

} // namespace

std::string ParsedToken::to_string() const {
    switch (type) {
        case TokenType::sum: return std::string("sum");
        case TokenType::sub: return std::string("sub");
        case TokenType::div: return std::string("div");
        case TokenType::mul: return std::string("mul");
        case TokenType::sin: return std::string("sin");
        case TokenType::cos: return std::string("cos");
        case TokenType::number: return fmt::format("num {}", value.value());
        case TokenType::variable: return fmt::format("var {}", value.value());
        case TokenType::negate: return std::string("neg");
        case TokenType::open_par: return std::string("open_par");
        case TokenType::close_par: return std::string("close_par");
        default: return std::string("error");
    }
}

std::ostream& operator<<(std::ostream& os, const ParsedToken& tok) {
    return os << fmt::format("({})", tok.to_string());
}

std::vector<std::string> splice_string(const std::string& input) {
    constexpr auto delimiters = "+-*/()";

    std::vector<std::string> result;
    std::size_t prev = 0, pos;

    while ((pos = input.find_first_of(delimiters, prev)) != std::string::npos) {
        if (pos > prev) {
            result.emplace_back(input.substr(prev, pos - prev));
        }
        auto op_token = input.substr(pos, 1);
        // unary minus can be at the start of the expression so better check if result is empty
        if (op_token == "-"
            && (result.empty()
                || (!is_number(result.back()) && !is_variable(result.back()) && !is_function(result.back())
                    && !is_close_par(result.back()))))
        {
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

std::vector<ParsedToken> convert_to_tokens(const std::vector<std::string>& spliced) {
    std::vector<ParsedToken> result;
    result.reserve(spliced.size());

    for (auto& spliced_string : spliced) {
        if (is_number(spliced_string)) {
            result.emplace_back(ParsedToken {TokenType::number, spliced_string});
        } else if (is_variable(spliced_string) && !is_function(spliced_string)) {
            result.emplace_back(ParsedToken {TokenType::variable, spliced_string});
        } else if (is_open_par(spliced_string)) {
            result.emplace_back(ParsedToken {TokenType::open_par, std::nullopt});
        } else if (is_close_par(spliced_string)) {
            result.emplace_back(ParsedToken {TokenType::close_par, std::nullopt});
        } else {
            auto token_type = token_operator_type(spliced_string);
            if (token_type != TokenType::err) {
                result.emplace_back(ParsedToken {token_type, std::nullopt});
            } else {
                fmt::print(stderr, "Token not recognized : \"{}\"", spliced_string);
            }
        }
    }

    return result;
}

// also known as shunting yard algorithm
std::vector<ParsedToken> convert_to_reverse_notation(const std::vector<ParsedToken>& tokens) {
    std::vector<ParsedToken> result;
    result.reserve(tokens.size());
    Stack<ParsedToken> stack;

    const std::unordered_map<TokenType, int> OPERATOR_PRIORITY {
        {TokenType::sum, 1},
        {TokenType::sub, 1},
        {TokenType::mul, 2},
        {TokenType::div, 2},
        {TokenType::negate, 3},
        {TokenType::cos, 3},
        {TokenType::sin, 3},
        {TokenType::open_par, 0},
        {TokenType::close_par, 0},
    };


    for (const auto& token : tokens) {
        if (token.type == TokenType::number || token.type == TokenType::variable) {
            result.emplace_back(token);
        } else if (is_operator(token) || is_parenthesis(token) || is_function(token)) {
            if (token.type != TokenType::open_par) {
                // pop stack until :
                // '(' met if current token is ')'
                // or if op_priority(token) >= op_priority(stack.top)
                while (!stack.empty()
                       && ((token.type != TokenType::close_par && stack.top().type != TokenType::open_par)
                           || (OPERATOR_PRIORITY.at(stack.top().type) > OPERATOR_PRIORITY.at(token.type))))
                {
                    result.emplace_back(stack.pop());
                }

                if (token.type == TokenType::close_par && !stack.empty()) {
                    stack.pop();
                }
            }

            if (token.type != TokenType::close_par) {
                stack.push(token);
            }
        }
    }
    while (!stack.empty()) {
        auto token_from_stack = stack.pop();
        if (!is_parenthesis(token_from_stack)) {
            result.emplace_back(token_from_stack);
        }
    }

    return result;
}

std::shared_ptr<Expression> reverse_notation_to_expression(const std::vector<ParsedToken>& parsed_tokens) {
    Stack<std::shared_ptr<Expression>> temp;
    temp.reserve(parsed_tokens.size());

    for (auto& token : parsed_tokens) {
        if (is_unary_or_function(token.type)) {
            temp.push(function_token_to_expression(token.type, temp.pop()));
        } else if (is_operator(token.type) && temp.size() >= 2) {
            auto rhs = temp.pop();
            auto lhs = temp.pop();
            temp.push(token_to_expression(token.type, lhs, rhs));
        } else {
            temp.push(token_to_expression(token));
        }
    }

    return temp.pop();
}
} // namespace parser::internal