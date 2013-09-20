#ifndef MATH_TOGGLEBUTTON_H
#define MATH_TOGGLEBUTTON_H

#include <string>
#include <functional>
#include "cocos2d.h"

class ToggleButton : public cocos2d::Layer
{
public:
    std::function<bool ()> detectState = []() { return false; };
    std::function<std::string (const bool)> getLabel = [](const bool flag) { return ""; };
    std::function<bool (const bool)> toggleAction = [](const bool flag) { return false; };

    CREATE_FUNC(ToggleButton);
    bool init() override;

    void onEnter() override;
    void onExit() override;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    cocos2d::LabelTTF* label = nullptr;
    void addBackground();
    void addLabel();
};

#endif // MATH_TOGGLEBUTTON_H