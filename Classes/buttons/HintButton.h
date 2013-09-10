#ifndef MATH_HINTBUTTON_H
#define MATH_HINTBUTTON_H

#include "cocos2d.h"
#include <avalon/payment.h>

class GamePage;

class HintButton : public cocos2d::Node, public cocos2d::TouchDelegate,  public avalon::payment::ManagerDelegate
{
public:
    constexpr static float alertDelay = 15.0f;

    static HintButton* create(void) = delete;
    static HintButton* create(GamePage& game);
    bool init(GamePage& game);

    void onEnter() override;
    void onExit() override;
    bool ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

    void onPurchaseSucceed(avalon::payment::Manager* const manager, avalon::payment::Product* const product) override;
    void onPurchaseFail(avalon::payment::Manager* const manager) override;

    void onTransactionStart(avalon::payment::Manager* const manager) override;
    void onTransactionEnd(avalon::payment::Manager* const manager) override;

    void alert();

private:
    GamePage* game = nullptr;
    cocos2d::LabelTTF* label = nullptr;
    cocos2d::Sprite* key = nullptr;

    void addLabel();
    void addIcon();
};

#endif // MATH_HINTBUTTON_H
