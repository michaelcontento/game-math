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

const float getTouchIgnoreLength()
{
    return 25 * getScaleFactor();
}

const float getSwipeMovementMinX()
{
    return 10 * getScaleFactor();
}

const float getSwipeMovementMaxY()
{
    return 10 * getScaleFactor();
}

const float getQuestionStartDelay()
{
    return getQuestionFadeTime() * 2;
}

} // namespace config