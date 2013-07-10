#ifndef MATH_UTILS_CONFIG_H
#define MATH_UTILS_CONFIG_H

#include "cocos2d.h"

namespace config {

float getScaleFactor();
const cocos2d::Size& getFrameSize();
constexpr float getDesignWidth();
const float getDelayAfterScrollAnimation();
const float getTouchIgnoreLength();
const float getSnapAnimationDuration();
const float getSwipeMovementMinX();
const float getSwipeMovementMaxY();

}; // namespace config

#endif // MATH_UTILS_CONFIG_H