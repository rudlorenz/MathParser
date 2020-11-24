#pragma once
#include "Expression.h"

#include <memory>

class BinaryExpression : public Expression
{
public:
    BinaryExpression(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : lhs_(std::move(lhs))
        , rhs_(std::move(rhs))
    {}

    bool contains_var(const std::string_view var) const override
    {
        return lhs_->contains_var(var) || rhs_->contains_var(var);
    }

protected:
    std::shared_ptr<Expression> lhs_;
    std::shared_ptr<Expression> rhs_;
};
