#ifndef MATH_MAINPAGE_H
#define MATH_MAINPAGE_H

#include "cocos2d.h"
#include "Page.h"

class MainPage : public Page
{
public:
    CREATE_FUNC(MainPage);
    bool init() override;
    void onTouch(cocos2d::Touch& touch, cocos2d::Event& event) override;

    virtual void onBackspace() override;

private:
    void addIcon();
    void addTapToPlayLabel();
};

#endif // MATH_MAINPAGE_H