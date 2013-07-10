#include "color.h"

using namespace cocos2d;
using namespace cocos2d::extension;

namespace color {

cocos2d::extension::HSV toHSV(const cocos2d::ccColor3B& color)
{
    return ControlUtils::HSVfromRGB({color.r / 255.0, color.g / 255.0, color.b / 255.0, 1.0});
}

cocos2d::ccColor3B toRGB(const cocos2d::extension::HSV& color)
{
    auto rgba = ControlUtils::RGBfromHSV(color);
    return {GLubyte(rgba.r * 255), GLubyte(rgba.g * 255), GLubyte(rgba.b * 255)};
}

cocos2d::ccColor4F toRGBA(const cocos2d::extension::HSV& color)
{
    auto rgba = ControlUtils::RGBfromHSV(color);
    return {GLfloat(rgba.r), GLfloat(rgba.g), GLfloat(rgba.b), 1.0};
}

} // namespace color