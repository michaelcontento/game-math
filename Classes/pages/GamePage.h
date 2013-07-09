#ifndef MATH_GAMEPAGE_H
#define MATH_GAMEPAGE_H

#include "Page.h"

class GamePage : public Page
{
public:
    CREATE_FUNC(GamePage);
    bool init() override;
    void onTouch(cocos2d::Touch* touch, cocos2d::Event* event) override;
};

#endif // MATH_GAMEPAGE_H