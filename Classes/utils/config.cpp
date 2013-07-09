#include "config.h"

using namespace cocos2d;

namespace config {

float getScaleFactor()
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

} // namespace config