#ifndef EXPRESSIONS_SIN_H
#define EXPRESSIONS_SIN_H


#include "Expression.h"
#include "Mul.h"
#include "Sub.h"
#include <cmath>

class Sin : public Expression
{
public:
    Sin(Expression* value) : value_(value) {};

    Expression *diff() const override;
    std::string tostring() const override;
    double evaluate(double x) const override;

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
    double evaluate(double x) const override;

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

double Cos::evaluate(double x) const
{
    return cos(x);
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

double Sin::evaluate(double x) const
{
    return sin(x);
}


#endif //EXPRESSIONS_SIN_H
