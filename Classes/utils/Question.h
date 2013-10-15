#ifndef MATH_PUZZLE_QUESTION_H
#define MATH_PUZZLE_QUESTION_H

#include <string>
#include <cstddef>
#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>
#include <avalon/utils/platform.h>

class Question
{
public:
    std::string question;
    std::string rightAnswer;
    std::string wrongAnswer1;
    std::string wrongAnswer2;

    Question(std::string question, std::string rightAnswer, std::string wrongAnswer1, std::string wrongAnswer2)
    : question(question), rightAnswer(rightAnswer), wrongAnswer1(wrongAnswer1), wrongAnswer2(wrongAnswer2)
    {
        BOOST_ASSERT_MSG(rightAnswer != question, rightAnswer.c_str());
        BOOST_ASSERT_MSG(rightAnswer != wrongAnswer1, rightAnswer.c_str());
        BOOST_ASSERT_MSG(rightAnswer != wrongAnswer2, rightAnswer.c_str());
        BOOST_ASSERT_MSG(wrongAnswer1 != wrongAnswer2, wrongAnswer1.c_str());

        format(this->question);
        format(this->rightAnswer);
        format(this->wrongAnswer1);
        format(this->wrongAnswer2);
    }

    bool operator==(const Question& other) const
    {
        return (question == other.question && rightAnswer == other.rightAnswer);
    }

private:
    void format(std::string& str) const
    {
        boost::replace_all(str, "*", "×");
        boost::replace_all(str, "/", "÷");
        boost::replace_all(str, "+", "+");
#if !AVALON_PLATFORM_IS_TIZEN
        boost::replace_all(str, "-", "−");
#endif

        while (str.find("POW#") != std::string::npos) {
            boost::replace_all(str, "POW#0", "⁰POW#");
            boost::replace_all(str, "POW#1", "¹POW#");
            boost::replace_all(str, "POW#2", "²POW#");
            boost::replace_all(str, "POW#3", "³POW#");
            boost::replace_all(str, "POW#4", "⁴POW#");
            boost::replace_all(str, "POW#5", "⁵POW#");
            boost::replace_all(str, "POW#6", "⁶POW#");
            boost::replace_all(str, "POW#7", "⁷POW#");
            boost::replace_all(str, "POW#8", "⁸POW#");
            boost::replace_all(str, "POW#9", "⁹POW#");
            boost::replace_all(str, "POW##", "");
        }
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
