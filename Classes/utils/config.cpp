#include "config.h"

using namespace cocos2d;

namespace config {

const float getScaleFactor()
{
    return getFrameSize().width / getDesignWidth();
}

const cocos2d::Size& getFrameSize()
{
    return Director::sharedDirector()->getOpenGLView()->getFrameSize();
}

constexpr float getDesignWidth()
{
    return 768;
}

const float getDelayAfterScrollAnimation()
{
    return 0.1;
}

const float getTouchIgnoreLength()
{
    return 25 * getScaleFactor();
}

const float getSnapAnimationDuration()
{
    return 0.7;
}

const float getSwipeMovementMinX()
{
    return 10 * getScaleFactor();
}

const float getSwipeMovementMaxY()
{
    return 10 * getScaleFactor();
}

const float getQuestionFadeTime()
{
    return 0.7;
}

const float getQuestionStartDelay()
{
    return getQuestionFadeTime() * 2;
}

const cocos2d::extension::HSV getHsvColorStep()
{
    return {10, 0, 0};
}

} // namespace config