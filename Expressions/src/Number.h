#ifndef EXPRESSIONS_NUMBER_H
#define EXPRESSIONS_NUMBER_H

#include "Expression.h"

class Number : public Expression
{
public:
    Number() : value_() {};
    explicit Number(const int& value) : value_(value) {};

    Expression* diff() const override;
    std::string tostring() const override;

    ~Number() override = default;

private:
    int value_;
};

Expression* Number::diff() const
{
    return new Number(0);
}

//should be RVO-d
std::string Number::tostring() const
{
    return std::to_string(value_);
}


#endif //EXPRESSIONS_NUMBER_H