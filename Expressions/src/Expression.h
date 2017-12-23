#ifndef EXPRESSIONS_EXPRESSION_H
#define EXPRESSIONS_EXPRESSION_H

#include <string>

class Expression
{

public:
    virtual Expression* diff() const = 0;
    virtual std::string tostring() const = 0;
    virtual double evaluate(double x) const = 0;

    virtual ~Expression() {};
};

#endif //EXPRESSIONS_EXPRESSION_H