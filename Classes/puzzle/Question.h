#ifndef MATH_PUZZLE_QUESTION_H
#define MATH_PUZZLE_QUESTION_H

#include <string>
#include <cstddef>

namespace puzzle {

class Question
{
public:
    const std::string question;
    const std::string rightAnswer;
    const std::string wrongAnswer1;
    const std::string wrongAnswer2;

    Question(std::string question, std::string rightAnswer, std::string wrongAnswer1, std::string wrongAnswer2)
    : question(question), rightAnswer(rightAnswer), wrongAnswer1(wrongAnswer1), wrongAnswer2(wrongAnswer2)
    {
    }

    bool operator==(const Question& other) const
    {
        return (question == other.question);
    }
};

} // namespace puzzle


namespace std {

template <>
struct hash<puzzle::Question>
{
    std::size_t operator ()(const puzzle::Question& value) const
    {
        return hash<string>()(value.question);
    }
};

string to_string(const puzzle::Question& q)
{
    return "Q: " + q.question
        + " A: [" + q.rightAnswer + "] " + q.wrongAnswer1 + " " + q.wrongAnswer2;
}

} // namespace std

#endif // MATH_PUZZLE_QUESTION_H
