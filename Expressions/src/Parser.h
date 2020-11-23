#pragma once

#include "Expression.h"

#include <string>
#include <memory>
#include <vector>

std::shared_ptr<Expression> parse(std::string input);
