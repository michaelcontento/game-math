#ifndef MATH_SETTINGSPAGE_H
#define MATH_SETTINGSPAGE_H

#include "Page.h"

class ToggleButton;

class SettingsPage : public Page
{
public:
    CREATE_FUNC(SettingsPage);
    bool init() override;

private:
    cocos2d::Node* container = nullptr;

    void addButtons();
    void updateContainerLayout() const;

    Node* getBlankButton() const;
    ToggleButton* getSoundButton() const;
    ToggleButton* getAchievementsButton() const;
    ToggleButton* getRestoreButton() const;
    ToggleButton* getRemoveAdsButton() const;
    ToggleButton* getUnlockAllButton() const;
};

#endif // MATH_SETTINGSPAGE_H