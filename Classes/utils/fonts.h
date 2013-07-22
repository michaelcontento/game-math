#ifndef MATH_FONTS_H
#define MATH_FONTS_H

#include <string>
#include <deque>
#include "cocos2d.h"

namespace fonts {

cocos2d::LabelTTF* createNormal(const std::string& text, const unsigned short size);
cocos2d::LabelTTF* createLight(const std::string& text, const unsigned short size);
cocos2d::Sprite* createStar(const cocos2d::Color3B& color);
void fillStarContainer(cocos2d::Node& cocosContainer, std::deque<cocos2d::Sprite*>& cppContainer, const int amount, const cocos2d::Color3B& color);

} // namespace fonts

#endif // MATH_FONTS_H