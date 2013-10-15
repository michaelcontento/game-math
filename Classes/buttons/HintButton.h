#ifndef MATH_HINTBUTTON_H
#define MATH_HINTBUTTON_H

#include "cocos2d.h"
#include <avalon/payment.h>

class GamePage;

class HintButton : public cocos2d::Layer, public avalon::payment::ManagerDelegate
{
public:
    HintButton()
    : game(nullptr)
    , label(nullptr)
    , key(nullptr)
    {};

    constexpr static float alertDelay = 15.0f;

    static HintButton* create(void) = delete;
    static HintButton* create(GamePage& game);
    bool init(GamePage& game);

    void onEnter() override;
    void onExit() override;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

    void onPurchaseSucceed(avalon::payment::Manager* const manager, avalon::payment::Product* const product) override;
    void onPurchaseFail(avalon::payment::Manager* const manager) override;

    void onTransactionStart(avalon::payment::Manager* const manager) override;
    void onTransactionEnd(avalon::payment::Manager* const manager) override;

    void alert();

private:
    GamePage* game;
    cocos2d::LabelTTF* label;
    cocos2d::Sprite* key;

    void addLabel();
    void addIcon();
};

#endif // MATH_HINTBUTTON_H
