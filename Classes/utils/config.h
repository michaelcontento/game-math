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

using diceFunction = std::function<int(const std::unordered_set<int>)>;
diceFunction createDice(const int min, const int max);
diceFunction createDiceRange(const int pos);
std::function<Question()> getGenerator(const int group, const int level);
std::function<Question()> getGeneratorAddition(const int number, const bool easy);
std::function<Question()> getGeneratorSubtraction(const int number, const bool easy);
std::function<Question()> getGeneratorMultiplication(const int number, const bool easy);
std::function<Question()> getGeneratorDivision(const int number, const bool easy);
std::function<Question()> getGeneratorPowers(const int number, const bool easy);

constexpr float getDesignWidth()
{
    return 768;
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
    return 0.8;
}

constexpr float getAlertFadeTime()
{
    return 0.7;
}

constexpr cocos2d::extension::HSV getHsvColorStep()
{
    return {0, 0, -0.20};
}

}; // namespace config

#endif // MATH_CONFIG_H