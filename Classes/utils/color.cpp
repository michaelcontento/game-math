#include "color.h"

using namespace cocos2d;
using namespace cocos2d::extension;

namespace color {

cocos2d::extension::HSV toHSV(const cocos2d::Color3B& color)
{
    return ControlUtils::HSVfromRGB({color.r / 255.0, color.g / 255.0, color.b / 255.0, 1.0});
}

cocos2d::Color3B toRGB(const cocos2d::extension::HSV& color)
{
    auto rgba = ControlUtils::RGBfromHSV(color);
    return {GLubyte(rgba.r * 255), GLubyte(rgba.g * 255), GLubyte(rgba.b * 255)};
}

cocos2d::Color4F toRGBA(const cocos2d::extension::HSV& color)
{
    auto rgba = ControlUtils::RGBfromHSV(color);
    return {GLfloat(rgba.r), GLfloat(rgba.g), GLfloat(rgba.b), 1.0};
}

cocos2d::Color3B toRGB(const cocos2d::Color4F& color)
{
    return {GLubyte(color.r * 255), GLubyte(color.g * 255), GLubyte(color.b * 255)};
}
    
cocos2d::Color4F toRGBA(const cocos2d::Color3B& color)
{
    return {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f};
}

} // namespace color