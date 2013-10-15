#ifndef MATH_BACKBUTTON_H
#define MATH_BACKBUTTON_H

#include "cocos2d.h"

class BackButton : public cocos2d::Layer
{
public:
    BackButton()
    : btn(nullptr)
    {};

    CREATE_FUNC(BackButton);

    bool init() override;
    void onEnter() override;
    void onExit() override;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    cocos2d::Sprite* btn;
};

#endif // MATH_BACKBUTTON_H
