#ifndef MATH_TOGGLEBUTTON_H
#define MATH_TOGGLEBUTTON_H

#include <string>
#include <functional>
#include "cocos2d.h"

class ToggleButton : public cocos2d::Layer
{
public:
    ToggleButton()
    : detectState([]() { return false; })
    , getLabel([](const bool flag) { return ""; })
    , toggleAction([](const bool flag) { return false; })
    , label(nullptr)
    {};

    std::function<bool ()> detectState;
    std::function<std::string (const bool)> getLabel;
    std::function<bool (const bool)> toggleAction;

    CREATE_FUNC(ToggleButton);
    bool init() override;

    void onEnter() override;
    void onExit() override;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    cocos2d::LabelTTF* label;
    void addBackground();
    void addLabel();
};

#endif // MATH_TOGGLEBUTTON_H
