#pragma once

#include "Expression.h"

#include <string>
#include <optional>

namespace parser
{

std::optional<mexpr::Expression> parse(std::string input);

} // namespace parser