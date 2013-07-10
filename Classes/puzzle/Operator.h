#ifndef MATH_PUZZLE_OPERATOR_H
#define MATH_PUZZLE_OPERATOR_H

#include <cstddef>

namespace puzzle {

enum class Operator : unsigned short
{
    PLUS, MINUS, MULTIPLICATION, DIVISION
};

} // namespace puzzle



namespace std {

template <>
struct hash<puzzle::Operator>
{
    std::size_t operator ()(puzzle::Operator value) const
    {
        return static_cast<std::size_t>(value);
    }
};
    
} // namespace std

#endif // MATH_PUZZLE_OPERATOR_H