#ifndef MATH_PUZZLE_QUESTION_H
#define MATH_PUZZLE_QUESTION_H

#include <string>

namespace puzzle {

class Question
{
public:
    const std::string question;
    const std::string rightAnswer;
    const std::string wrongAnswer1;
    const std::string wrongAnswer2;

    Question(const std::string& question, const std::string& rightAnswer, const std::string& wrongAnswer1, const std::string& wrongAnswer2)
    : question(question), rightAnswer(rightAnswer), wrongAnswer1(wrongAnswer1), wrongAnswer2(wrongAnswer2)
    {}
};

} // namespace puzzle

#endif // MATH_PUZZLE_QUESTION_H
