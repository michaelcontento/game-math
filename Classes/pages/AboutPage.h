#ifndef MATH_ABOUTPAGE_H
#define MATH_ABOUTPAGE_H

#include "cocos2d.h"
#include "Page.h"
#include <avalon/payment.h>

class ToggleButton;

class AboutPage : public Page, public avalon::payment::ManagerDelegate
{
public:
    AboutPage()
    : icon(nullptr)
    , desc(nullptr)
    , container(nullptr)
    , visible(false)
    , touchable(true)
    , purchased(false)
    {};

    CREATE_FUNC(AboutPage);
    bool init() override;

    void onTouch(cocos2d::Touch& touch, cocos2d::Event& event) override;
    
    void onPurchaseSucceed(avalon::payment::Manager* const manager, avalon::payment::Product* const product) override;
    void onPurchaseFail(avalon::payment::Manager* const manager) override;

    void onTransactionStart(avalon::payment::Manager* const manager) override;
    void onTransactionEnd(avalon::payment::Manager* const manager) override;

private:
    cocos2d::Sprite* icon;
    cocos2d::LabelTTF* desc;
    cocos2d::Node* container;
    bool visible;
    bool touchable;
    bool purchased;

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
