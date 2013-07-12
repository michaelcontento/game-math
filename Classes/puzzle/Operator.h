#ifndef MATH_PUZZLE_OPERATOR_H
#define MATH_PUZZLE_OPERATOR_H

#include <string>
#include <cstddef>

namespace puzzle {

enum class Operator : unsigned short
{
    PLUS, MINUS, MULTIPLICATION, DIVISION
};

int calculate(const Operator& op, const int a, const int b)
{
    switch (op) {
    case Operator::PLUS:
        return a + b;
    case Operator::MINUS:
        return a - b;
    case Operator::MULTIPLICATION:
        return a * b;
    case Operator::DIVISION:
        return a / b;
    }
}

} // namespace puzzle



namespace std {

template <>
struct hash<puzzle::Operator>
{
    size_t operator ()(const puzzle::Operator& value) const
    {
        return static_cast<size_t>(value);
    }
};

string to_string(const puzzle::Operator& op)
{
    switch (op) {
    case puzzle::Operator::PLUS:
        return "+";
    case puzzle::Operator::MINUS:
        return "-";
    case puzzle::Operator::MULTIPLICATION:
        return "x";
    case puzzle::Operator::DIVISION:
        return "/";
    }
}
    
} // namespace std

#endif // MATH_PUZZLE_OPERATOR_H