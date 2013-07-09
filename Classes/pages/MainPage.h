#ifndef MATH_MAINPAGE_H
#define MATH_MAINPAGE_H

#include "Page.h"

class MainPage : public Page
{
public:
    CREATE_FUNC(MainPage);
    bool init() override;
    void onTouch(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    void addTapToPlayLabel();
};

#endif // MATH_MAINPAGE_H