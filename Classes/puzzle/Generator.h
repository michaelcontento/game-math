#ifndef MATH_PUZZLE_GENERATOR_H
#define MATH_PUZZLE_GENERATOR_H

#include <string>
#include <unordered_set>
#include "Question.h"
#include "Operator.h"
#include "NumberRange.h"

namespace puzzle {

class Generator
{
public:
    const std::string question;
    const std::string answer;
    const std::unordered_set<Operator> operators;
    const std::unordered_set<NumberRange> ranges;

    Generator(const std::string& question, const std::string& answer, const std::unordered_set<Operator>& operators, const std::unordered_set<NumberRange>& ranges)
    : question(question), answer(answer), operators(operators), ranges(ranges)
    {}

    const Question generate() const;
};

} // namespace puzzle

#endif // MATH_PUZZLE_GENERATOR_H

/**
 * [addition-01]
 * Q = 11 + 11
 * A = 11
 *
 * [addition-02]
 * Q = 11 + ? = 11
 * A = 11
 *
 * [addition-03]
 * Q = 11
 * A = 11 + 11
 *
 * [addition-04]
 * Q = Smallest?
 * A = 11 + 11
 *
 * [addition-05]
 * Q = 11 + 11 + 11
 * A = 11
 *
 * [addition-06]
 * Q = 11 + 11
 * A = 11 + 11
 *
 * [addition-07]
 * Q = Largest?
 * A = 11 + 11
 *
 * [addition-08]
 * Q = MIXED 11
 * A = MIXED 11
 * 
 * !! MULT-04a: 11 x (11 +- 11)
 */
