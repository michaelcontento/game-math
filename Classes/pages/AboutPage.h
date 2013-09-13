#ifndef MATH_ABOUTPAGE_H
#define MATH_ABOUTPAGE_H

#include "cocos2d.h"
#include "Page.h"
#include <avalon/payment.h>

class ToggleButton;

class AboutPage : public Page, public avalon::payment::ManagerDelegate
{
public:
    CREATE_FUNC(AboutPage);
    bool init() override;

    void onTouch(cocos2d::Touch& touch, cocos2d::Event& event) override;

    void onPurchaseSucceed(avalon::payment::Manager* const manager, avalon::payment::Product* const product) override;
    void onPurchaseFail(avalon::payment::Manager* const manager) override;

    void onTransactionStart(avalon::payment::Manager* const manager) override;
    void onTransactionEnd(avalon::payment::Manager* const manager) override;

private:
    cocos2d::Sprite* icon = nullptr;
    cocos2d::LabelTTF* desc = nullptr;
    cocos2d::Node* container = nullptr;
    bool visible = false;
    bool touchable = true;
    bool purchased = false;

    void toggleVisibility();
    void initIcon();
    void initDesc();
    void addDescription();
    void addButtons();
    void updateContainerLayout() const;
    ToggleButton* getOurAppsButton() const;
    ToggleButton* getContactUsButton() const;
    ToggleButton* getDonateButton();
    ToggleButton* getFacebookButton() const;
};

#endif // MATH_ABOUTPAGE_H