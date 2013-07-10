#ifndef MATH_LEVELBUTTON_H
#define MATH_LEVELBUTTON_H

#include "cocos2d.h"

class Page;

class LevelButton : public cocos2d::Node
{
public:
    static LevelButton* create(void) = delete;
    static LevelButton* create(const int number, const Page* const parentPage);
    bool init(const int number, const Page* const parentPage);

    void onTouch(cocos2d::Touch* touch, cocos2d::Event* event);

private:
    int number = -1;
    const Page* parentPage;

    void configureSize();
    void addBackground();
    void addNumber();
    bool hasBeenTouched(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // MATH_LEVELBUTTON_H