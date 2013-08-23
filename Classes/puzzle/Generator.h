#ifndef MATH_PUZZLE_GENERATOR_H
#define MATH_PUZZLE_GENERATOR_H

#include <string>
#include <unordered_set>
#include <random>
#include <utility>
#include "Question.h"
#include "Operator.h"
#include "NumberRange.h"

namespace puzzle {

class Generator
{
public:
    std::unordered_set<Operator> operators;
    std::unordered_set<NumberRange> ranges;

    Generator(std::unordered_set<Operator> operators, std::unordered_set<NumberRange> ranges)
    : operators(operators), ranges(ranges)
    {}
    virtual ~Generator() {};

    virtual Question generate() = 0;

protected:
    static std::random_device randDevice;

    int dice(const std::unordered_set<int> ignore = {});
};

class Generator01 : public Generator
{
public:
    Generator01(std::unordered_set<Operator> operators, std::unordered_set<NumberRange> ranges)
    : Generator(operators, ranges)
    {}
    
    Question generate() override;
};

class Generator02 : public Generator
{
public:
    Generator02(std::unordered_set<Operator> operators, std::unordered_set<NumberRange> ranges)
    : Generator(operators, ranges)
    {}

    Question generate() override;
};

class Generator03 : public Generator
{
public:
    Generator03(std::unordered_set<Operator> operators, std::unordered_set<NumberRange> ranges)
    : Generator(operators, ranges)
    {}

    Question generate() override;
};

class Generator04 : public Generator
{
public:
    Generator04(std::unordered_set<Operator> operators, std::unordered_set<NumberRange> ranges)
    : Generator(operators, ranges)
    {}

    Question generate() override;
};

class Generator05 : public Generator
{
public:
    Generator05(std::unordered_set<Operator> operators, std::unordered_set<NumberRange> ranges)
    : Generator(operators, ranges)
    {}

    Question generate() override;
};

class Generator06 : public Generator
{
public:
    Generator06(std::unordered_set<Operator> operators, std::unordered_set<NumberRange> ranges)
    : Generator(operators, ranges)
    {}

    Question generate() override;
};

class Generator07 : public Generator
{
public:
    Generator07(std::unordered_set<Operator> operators, std::unordered_set<NumberRange> ranges)
    : Generator(operators, ranges)
    {}

    Question generate() override;
};

class Generator08 : public Generator
{
public:
    Generator08(std::unordered_set<Operator> operators, std::unordered_set<NumberRange> ranges)
    : Generator(operators, ranges)
    {}

    Question generate() override;
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
 *
 * Generator g1 {""{number} {operator} {number}", "{number}"};
 * Generator g2 {"{number} {operator} {hidden}", "{hidden}"};
 * Generator g3 {"{answer}", "{number} {operator} {number}"};
 * Generator g4 {"numberest?", "{number} {operator} {number}"}; // ?
 * Generator g5 {"{number} {operator} {number} {operator} {number}", "{number}"};
 * Generator g6 {"{number} {operator} {number}", "{number} {operator} {number}"}; // ?
 * Generator g7 {"Largest?", "{number} {operator} {number}"}; // ?
 * GeneratorMixer g8 {g1, g2, g3, g4, g5, g6, g7};
 */
