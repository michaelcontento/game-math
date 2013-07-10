#ifndef MATH_PUZZLE_NUMBERRANGE_H
#define MATH_PUZZLE_NUMBERRANGE_H

#include <cstddef>

namespace puzzle {

enum class NumberRange : unsigned short
{
    SMALL, MEDIUM
};

} // namespace puzzle



namespace std {

template <>
struct hash<puzzle::NumberRange>
{
    std::size_t operator ()(puzzle::NumberRange value) const
    {
        return static_cast<std::size_t>(value);
    }
};

} // namespace std

#endif // MATH_PUZZLE_NUMBERRANGE_H