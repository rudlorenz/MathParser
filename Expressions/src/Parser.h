#pragma once

#include "Expressions.h"

#include <memory>
#include <string>
#include <vector>

namespace parser
{

std::shared_ptr<Expression> parse(std::string input);

} // namespace parser