#ifndef MATH_COLOR_H
#define MATH_COLOR_H

#include "cocos2d.h"
#include "cocos-ext.h"

namespace color {

cocos2d::extension::HSV toHSV(const cocos2d::Color3B& color);
cocos2d::Color3B toRGB(const cocos2d::extension::HSV& color);
cocos2d::Color4F toRGBA(const cocos2d::extension::HSV& color);

}; // namespace color

#endif // MATH_COLOR_H