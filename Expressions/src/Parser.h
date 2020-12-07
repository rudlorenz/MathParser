#pragma once

#include "Expressions.h"

#include <string>
#include <memory>
#include <vector>

namespace parser
{

std::shared_ptr<Expression> parse(std::string input);

} // namespace parser