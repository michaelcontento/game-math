#include "config.h"

using namespace cocos2d;

namespace config {

float getScaleFactor()
{
    constexpr float designWidth = 768;
    return EGLView::sharedOpenGLView()->getFrameSize().width / designWidth;
}

const cocos2d::Size& getFrameSize()
{
    return Director::sharedDirector()->getOpenGLView()->getFrameSize();
}


} // namespace config