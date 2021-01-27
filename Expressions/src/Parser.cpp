#include "Parser.h"

#include "ParserInternal.h"
#include "Stack.h"

namespace parser
{

std::shared_ptr<Expression> parse(std::string input) {
    input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());

    const auto input_tokens = internal::convert_to_tokens(internal::splice_string(input));
    const auto tokens = internal::convert_to_reverse_notation(input_tokens);

    return internal::reverse_notation_to_expression(tokens);
}

} // namespace parser