#include "ParserInternal.h"

#include "Stack.h"

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <unordered_map>

namespace parser::internal
{

using mexpr::Expression;
using mexpr::unary_expr_type;
using mexpr::binary_expr_type;

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
    switch (token.type) {
        case TokenType::sum:
        case TokenType::sub:
        case TokenType::mul:
        case TokenType::div:
        case TokenType::negate: return true;
        default: return false;
    }
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

bool is_parenthesis(const ParsedToken& token) {
    return token.type == TokenType::open_par || token.type == TokenType::close_par;
}

Expression token_to_expression(const ParsedToken& token) {
    switch (token.type) {
        case TokenType::number: return Expression{std::stoi(token.value.value())};
        case TokenType::variable: return Expression{token.value.value()};
        default:{
            fmt::format("Error converting value token. Unexpected token : {}", token);
            return Expression{"ERR"};
        }
    }
}

Expression function_token_to_expression(const ParsedToken& token, Expression&& param) {
    switch (token.type) {
        case TokenType::negate: return Expression{unary_expr_type::unary_minus, std::move(param)};
        case TokenType::sin: return Expression{unary_expr_type::sin, std::move(param)};
        case TokenType::cos: return Expression{unary_expr_type::cos, std::move(param)};
        default:{
            fmt::format("Error converting unary / function token. Unexpected token : {}", token);
            return Expression{"ERR"};
        }
    }
}

Expression token_to_expression(const ParsedToken& token, Expression&& lhs, Expression&& rhs) {
    switch (token.type) {
        case TokenType::sum: return Expression{binary_expr_type::sum, std::move(lhs), std::move(rhs)};
        case TokenType::sub: return Expression{binary_expr_type::sub, std::move(lhs), std::move(rhs)};
        case TokenType::div: return Expression{binary_expr_type::div, std::move(lhs), std::move(rhs)};
        case TokenType::mul: return Expression{binary_expr_type::mul, std::move(lhs), std::move(rhs)};
        default:{
            fmt::format("Error converting operator token. Unexpected token : {}", token);
            return Expression{"ERR"};
        }
    }
}

bool is_unary_or_function(const ParsedToken& token) {
    switch (token.type) {
        case TokenType::negate:
        case TokenType::sin:
        case TokenType::cos: return true;
        default: return false;
    }
}

} // namespace

int ParsedToken::priority() const {
    switch (type) {
        case TokenType::sum:
        case TokenType::sub: return 2;
        case TokenType::mul:
        case TokenType::div: return 3;
        case TokenType::negate:
        case TokenType::sin:
        case TokenType::cos: return 4;
        default: return 0;
    }
}

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

std::vector<ParsedToken> convert_to_reverse_notation(const std::vector<ParsedToken>& tokens) {
    // slightly modified shunting yard algorithm.

    // current state of state_machine
    enum class machine_state
    {
        process_operand,
        process_operator,
        error,
    } state = machine_state::process_operand;

    Stack<ParsedToken> stack;
    std::vector<ParsedToken> result;
    result.reserve(tokens.size());

    const auto process_operand = [&stack, &result](const auto& token) -> machine_state {
        if (is_unary_or_function(token) || is_parenthesis(token)) {
            stack.push(token);
            return machine_state::process_operand;
        } else if (token.type == TokenType::number || token.type == TokenType::variable) {
            result.emplace_back(token);
            return machine_state::process_operator;
        }
        return machine_state::error;
    };

    const auto process_operator = [&stack, &result](const auto& token) -> machine_state {
        if (token.type == TokenType::close_par) {
            if (stack.empty()) {
                fmt::print(stderr, "Error processing \")\" Operator stack is empty. Mismatched parenthesis?\n");
                return machine_state::error;
            }
            while (stack.top().type != TokenType::open_par) {
                result.emplace_back(stack.pop());
                if (stack.empty()) {
                    fmt::print(stderr, "Error processing \")\" Operator stack is empty. Mismatched parenthesis?\n");
                    return machine_state::error;
                }
            }
            stack.pop();
            return machine_state::process_operator;
        } else if (is_operator(token)) {
            while (!stack.empty()
                   && (stack.top().type != TokenType::open_par || stack.top().priority() > token.priority())) {
                result.emplace_back(stack.pop());
            }
            stack.push(token);
            return machine_state::process_operand;
        }
        return machine_state::error;
    };

    for (const auto& token : tokens) {
        switch (state) {
            case machine_state::process_operand: state = process_operand(token); break;
            case machine_state::process_operator: state = process_operator(token); break;
            case machine_state::error: {
                // in case of error - failfast from current function.
                fmt::print(stderr, "Error while processing {} token", token);
                return std::vector<ParsedToken>{};
            }
        }
    }

    for (auto&& token : stack) {
        if (!is_parenthesis(token)) {
            result.emplace_back(token);
        }
    }

    return result;
}

Expression reverse_notation_to_expression(const std::vector<ParsedToken>& parsed_tokens) {
    std::vector<Expression> temp;
    temp.reserve(parsed_tokens.size());

    // Expression is non-copyable :(
    for (auto&& token : parsed_tokens) {
        if (is_unary_or_function(token)) {
            auto last = std::move(temp[temp.size() - 1]);
            temp.pop_back();
            temp.emplace_back(function_token_to_expression(token, std::move(last)));
        } else if (is_operator(token) && temp.size() >= 2) {
            auto rhs = std::move(temp[temp.size() - 1]);
            temp.pop_back();
            auto lhs = std::move(temp[temp.size() - 1]);
            temp.pop_back();
            temp.emplace_back(token_to_expression(token, std::move(lhs), std::move(rhs)));
        } else {
            temp.emplace_back(token_to_expression(token));
        }
    }
    auto result = std::move(temp[0]);
    return result;
}

} // namespace parser::internal
