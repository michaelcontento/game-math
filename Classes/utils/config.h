#ifndef MATH_CONFIG_H
#define MATH_CONFIG_H

#include <string>
#include <functional>
#include <unordered_set>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Question.h"

namespace config {

float getScaleFactor();
float getScaleFactorHeight();
cocos2d::Size getFrameSize();
float getTouchIgnoreLength();
float getSwipeMovementMinX();
float getSwipeMovementMaxY();
float getQuestionStartDelay();
float getProgressbarHeight();
float getAnswerButtonPadding();
cocos2d::Size getAnswerButtonSize();
std::string getGroupHeadline(const int group);
cocos2d::Color3B getGroupColor(const int group);

typedef std::function<int(const std::unordered_set<int>)> diceFunction;
diceFunction createDice(const int min, const int max);
diceFunction createDiceRange(const int pos);
std::string formatFraction(const int a, const int b);
std::string formatFraction(const std::string a, const std::string b);
std::string formatPower(const int a);
std::string formatSqrt(const int a);
std::string formatSqrt(const std::string a);
void uniqueValues(const int& a, int& b, int& c);
std::function<Question()> getGenerator(const int group, const int level);
std::function<Question()> getGeneratorAddition(const int number, const bool easy);
std::function<Question()> getGeneratorSubtraction(const int number, const bool easy);
std::function<Question()> getGeneratorMultiplication(const int number, const bool easy);
std::function<Question()> getGeneratorDivision(const int number, const bool easy);
std::function<Question()> getGeneratorArithmetic(const int number, const bool easy);
std::function<Question()> getGeneratorFractions(const int number, const bool easy);
std::function<Question()> getGeneratorPowers(const int number, const bool easy);
std::function<Question()> getGeneratorStatistics(const int number, const bool easy);
std::function<Question()> getGeneratorEquations(const int number, const bool easy);
std::function<Question()> getGeneratorMixed(const int number, const bool easy);
cocos2d::extension::HSV getHsvColorStep();

constexpr float getDesignWidth()
{
    return 768;
}

constexpr float getDesignHeight()
{
    return 1024;
}

constexpr float getGameTime()
{
    return 100;
}

constexpr float getGameAlarmTime()
{
    return 25;
}

constexpr float getDelayAfterScrollAnimation()
{
    return 0.1;
}

constexpr float getSnapAnimationDuration()
{
    return 0.2;
}

constexpr float getScrollDownDuration()
{
    return 0.5;
}

constexpr float getQuestionFadeTime()
{
    return 0.4;
}

constexpr float getAlertFadeTime()
{
    return 0.7;
}

}; // namespace config

#endif // MATH_CONFIG_H
