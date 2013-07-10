#ifndef MATH_FONTS_H
#define MATH_FONTS_H

#include <string>
#include "cocos2d.h"

namespace fonts {

cocos2d::LabelTTF* createNormal(const std::string& text, const unsigned short size);
cocos2d::LabelTTF* createLight(const std::string& text, const unsigned short size);

} // namespace fonts

#endif // MATH_FONTS_H