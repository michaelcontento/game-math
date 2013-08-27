#ifndef MATH_PUZZLE_QUESTION_H
#define MATH_PUZZLE_QUESTION_H

#include <string>
#include <cstddef>

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
        return (question == other.question && rightAnswer == other.rightAnswer);
    }
};


namespace std {

template <>
struct hash<Question>
{
    std::size_t operator ()(const Question& value) const
    {
        return hash<string>()(value.question + value.rightAnswer);
    }
};

} // namespace std

#endif // MATH_PUZZLE_QUESTION_H
