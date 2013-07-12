#ifndef MATH_LEVELBUTTON_H
#define MATH_LEVELBUTTON_H

#include "cocos2d.h"

class Page;

class LevelButton : public cocos2d::Node
{
public:
    static LevelButton* create(void) = delete;
    static LevelButton* create(const short number, const Page& parentPage);
    bool init(const short number, const Page& parentPage);

    void onTouch(cocos2d::Touch& touch, cocos2d::Event& event);

private:
    short number = -1;
    const Page* parentPage = nullptr;

    void configureSize();
    void addBackground();
    const cocos2d::Color4F getBackgroundColorFromParentPage() const;
    void addNumber();
    bool hasBeenTouched(cocos2d::Touch& touch, cocos2d::Event& event);
};

#endif // MATH_LEVELBUTTON_H