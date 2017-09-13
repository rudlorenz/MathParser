#ifndef EXPRESSIONS_SIN_H
#define EXPRESSIONS_SIN_H


#include "Expression.h"
#include "Mul.h"
#include "Sub.h"

class Sin : public Expression
{
public:
    Sin(Expression* value) : value_(value) {};

    Expression *diff() const override;
    std::string tostring() const override;

    ~Sin() override;

private:
    Expression* value_;
};

class Cos : public Expression
{
public:
    Cos(Expression* value): value_(value) {};

    Expression *diff() const override;
    std::string tostring() const override;

    ~Cos() override;

private:
    Expression* value_;
};

Expression *Cos::diff() const
{
    return new Sum(
            value_->diff(),
            new Sub(new Number(0), new Sin(value_))
    );
}

std::string Cos::tostring() const
{
    return "cos(" + value_->tostring() + ")";
}

Cos::~Cos()
{
    delete value_;
}

Expression *Sin::diff() const
{
    return new Mul(value_->diff(), new Cos(value_));
}

std::string Sin::tostring() const
{
    return "sin(" + value_->tostring() + ")";
}

Sin::~Sin()
{
    delete value_;
}


#endif //EXPRESSIONS_SIN_H
