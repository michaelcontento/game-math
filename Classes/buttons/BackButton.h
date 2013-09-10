#ifndef MATH_BACKBUTTON_H
#define MATH_BACKBUTTON_H

#include "cocos2d.h"

class BackButton : public cocos2d::Node, public cocos2d::TouchDelegate
{
public:
    CREATE_FUNC(BackButton);

    bool init() override;
    void onEnter() override;
    void onExit() override;
    bool ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    cocos2d::Sprite* btn = nullptr;
};

#endif // MATH_BACKBUTTON_H