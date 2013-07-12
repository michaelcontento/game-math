#include "Generator.h"

#include <regex>
#include <boost/algorithm/string/replace.hpp>

namespace puzzle {

const std::string Generator::tokenDelimiterLeft  = "{";
const std::string Generator::tokenDelimiterRight = "}";
const std::string Generator::tokenNumber         = tokenDelimiterLeft + "number"   + tokenDelimiterRight;
const std::string Generator::tokenOperator       = tokenDelimiterLeft + "operator" + tokenDelimiterRight;
std::random_device Generator::randDevice;

Question Generator::generate()
{
    auto qa = generateQuestionAndAnswer();
    auto wrongAnswer = getWrongAnswers(qa.second);
    
    return Question(
        qa.first,
        std::to_string(qa.second),
        std::to_string(wrongAnswer.first),
        std::to_string(wrongAnswer.second)
    );
};

std::pair<std::string, int> Generator::generateQuestionAndAnswer()
{
    std::string realQuestion = question;
    int answer = 0;

    bool firstRun = true;
    while (realQuestion.find(tokenNumber) != std::string::npos) {
        auto op = Operator::PLUS;
        auto number = getNumber(op);

        auto valueNumber = std::to_string(number);
        boost::replace_first(realQuestion, tokenNumber, valueNumber);

        if (firstRun) {
            firstRun = false;
            answer = number;
        } else {
            answer = calculate(op, answer, number);
            boost::replace_first(realQuestion, tokenOperator, std::to_string(op));
        }
    }

    throwIfContainsUnhandledTokens(realQuestion);
    return std::make_pair(realQuestion, answer);
}

int Generator::getNumber(const puzzle::Operator& op)
{
    std::uniform_int_distribution<> dice(0, 12);
    return dice(randDevice);
}

void Generator::throwIfContainsUnhandledTokens(const std::string& checkStr) const
{
    std::regex regex {tokenDelimiterLeft + ".*" + tokenDelimiterRight};
    if (std::regex_match(checkStr, regex)) {
        throw new std::runtime_error("unhandled token left in question string");
    }
}

std::pair<int, int> Generator::getWrongAnswers(const int rightAnswer)
{
    int a = rightAnswer;
    while (a == rightAnswer) {
        a = getNumber(*operators.begin());
    }

    int b = rightAnswer;
    while (b == rightAnswer || b == a) {
        b = getNumber(*operators.begin());
    }

    return std::make_pair(a, b);
}

} // namespace puzzle