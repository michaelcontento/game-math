#ifndef MATH_MOREGAMESPAGE_H
#define MATH_MOREGAMESPAGE_H

#include "cocos2d.h"
#include "Page.h"

class MoreGamesPage : public Page
{
public:
    MoreGamesPage()
    : upperIcon(nullptr)
    , lowerIcon(nullptr)
    {};

    CREATE_FUNC(MoreGamesPage);
    bool init() override;

    void onTouch(cocos2d::Touch& touch, cocos2d::Event& event) override;

private:
    cocos2d::Sprite* upperIcon;
    cocos2d::Sprite* lowerIcon;

    void addUpperIcon();
    void addLowerIcon();
    void addDescription();
};

#endif // MATH_MOREGAMESPAGE_H
