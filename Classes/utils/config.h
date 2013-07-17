#ifndef MATH_CONFIG_H
#define MATH_CONFIG_H

#include "cocos2d.h"
#include "cocos-ext.h"

namespace config {

const float getScaleFactor();
const cocos2d::Size& getFrameSize();
const float getTouchIgnoreLength();
const float getSwipeMovementMinX();
const float getSwipeMovementMaxY();
const float getQuestionStartDelay();
const float getProgressbarHeight();

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
    return 0.5;
}

constexpr float getQuestionFadeTime()
{
    return 0.7;
}

constexpr cocos2d::extension::HSV getHsvColorStep()
{
    return {10, 0, 0};
}

}; // namespace config

#endif // MATH_CONFIG_H