#ifndef MATH_CONFIG_H
#define MATH_CONFIG_H

#include "cocos2d.h"
#include "cocos-ext.h"

namespace config {

const float getScaleFactor();
const cocos2d::Size& getFrameSize();
constexpr float getDesignWidth();
const float getDelayAfterScrollAnimation();
const float getTouchIgnoreLength();
const float getSnapAnimationDuration();
const float getSwipeMovementMinX();
const float getSwipeMovementMaxY();
const cocos2d::extension::HSV getHsvColorStep();

}; // namespace config

#endif // MATH_CONFIG_H