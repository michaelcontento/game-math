#ifndef MATH_SETTINGSPAGE_H
#define MATH_SETTINGSPAGE_H

#include "Page.h"
#include <avalon/payment.h>

class ToggleButton;

class SettingsPage : public Page, public avalon::payment::ManagerDelegate
{
public:
    CREATE_FUNC(SettingsPage);
    bool init() override;

    void onPurchaseSucceed(avalon::payment::Manager* const manager, avalon::payment::Product* const product) override;
    void onPurchaseFail(avalon::payment::Manager* const manager) override;

    void onTransactionStart(avalon::payment::Manager* const manager) override;
    void onTransactionEnd(avalon::payment::Manager* const manager) override;

    void onRestoreSucceed(avalon::payment::Manager* const manager) override;
    void onRestoreFail(avalon::payment::Manager* const manager) override;

private:
    cocos2d::Node* container = nullptr;
    constexpr static int tagRemoveAdsButton = 1;
    constexpr static int tagRestorePurchases = 2;
    constexpr static int tagUnlockAllButton = 3;

    void addButtons();
    void updateContainerLayout() const;

    Node* getBlankButton() const;
    ToggleButton* getSoundButton() const;
    ToggleButton* getAchievementsButton() const;
    ToggleButton* getRestoreButton();
    ToggleButton* getRemoveAdsButton();
    ToggleButton* getUnlockAllButton();
    void unlockPage(const int nbr, avalon::payment::Manager* const manager, avalon::payment::Product* const product);
};

#endif // MATH_SETTINGSPAGE_H