#ifndef MATH_UTILS_FONTS_H
#define MATH_UTILS_FONTS_H

#include <string>
#include "cocos2d.h"

namespace fonts {

cocos2d::LabelTTF* createNormal(const std::string text, const int size);
cocos2d::LabelTTF* createLight(const std::string text, const int size);

} // namespace fonts

#endif // MATH_UTILS_FONTS_H