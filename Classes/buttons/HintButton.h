#ifndef MATH_HINTBUTTON_H
#define MATH_HINTBUTTON_H

#include "cocos2d.h"

class GamePage;

class HintButton : public cocos2d::Node, public cocos2d::TouchDelegate
{
public:
    static HintButton* create(void) = delete;
    static HintButton* create(GamePage& game);
    bool init(GamePage& game);

    void onEnter() override;
    void onExit() override;
    bool ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    GamePage* game = nullptr;
    cocos2d::LabelTTF* label = nullptr;

    void addLabel();
};

#endif // MATH_HINTBUTTON_H
