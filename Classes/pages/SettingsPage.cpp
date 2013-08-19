#include "SettingsPage.h"

#include <list>
#include "../utils/config.h"
#include "../utils/user.h"
#include "../buttons/ToggleButton.h"

using namespace cocos2d;

bool SettingsPage::init()
{
    if (!Page::init()) {
        return false;
    }

    setBackground(Color3B::WHITE);
    addButtons();

    return true;
}

void SettingsPage::addButtons()
{
    const std::list<Node*> btns = {
        getUnlockAllButton(),
        getRemoveAdsButton(),
        getBlankButton(),
        getAchievementsButton(),
        getBlankButton(),
        getRestoreButton(),
        getSoundButton()
    };

    container = Node::create();
    addChild(container);

    for (const auto& btn : btns) {
        if (btn != nullptr) {
            container->addChild(btn);
        }
    }

    updateContainerLayout();
}

void SettingsPage::updateContainerLayout() const
{
    const auto spacing = 15 * config::getScaleFactor();
    float nextPosY = 0;
    float maxWidth = 0;
    bool lastNodeWasToggleButton = false;

    for (const auto& child : *container->getChildren()) {
        const auto btn = dynamic_cast<Node*>(child);
        if (!btn) {
            continue;
        }

        const auto isToggleButton = (dynamic_cast<ToggleButton*>(child) != nullptr);
        if (!lastNodeWasToggleButton && !isToggleButton) {
            container->removeChild(btn);
            continue;
        }
        lastNodeWasToggleButton = isToggleButton;

        btn->setAnchorPoint(Point::ZERO);
        btn->setPositionY(nextPosY);

        nextPosY += btn->getContentSize().height + spacing;
        maxWidth = fmaxf(maxWidth, btn->getContentSize().width);
    }

    container->setContentSize({maxWidth, nextPosY - spacing});
    container->setAnchorPoint({0.5, 0.5});
    container->setPosition(Point(config::getFrameSize() / 2));
}

ToggleButton* SettingsPage::getSoundButton() const
{
    const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return flag ? "sound on" : "sound off"; };
    btn->detectState = []() { return user::hasSoundEnabled(); };
    btn->toggleAction = [](const bool flag) { user::setSoundEnabled(flag); return true; };

    return btn;
}

ToggleButton* SettingsPage::getAchievementsButton() const
{
    const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return "achievements"; };
    btn->toggleAction = [](const bool flag) { log("ACHIEVEMENTS!"); return false; };

    return btn;
}

ToggleButton* SettingsPage::getRestoreButton() const
{
    const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return "restore my purchases"; };
    btn->toggleAction = [](const bool flag) { log("RESTORE!"); return false; };

    return btn;
}

ToggleButton* SettingsPage::getRemoveAdsButton() const
{
    if (!user::hasAdsEnabled()) {
        return nullptr;
    }
    
    const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return "remove all ads"; };
    btn->toggleAction = [this, btn](const bool flag) {
        user::setAdsEnabled(false);
        container->removeChild(btn);
        updateContainerLayout();
        return false;
    };
    
    return btn;
}

ToggleButton* SettingsPage::getUnlockAllButton() const
{
    const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return "unlock all"; };
    btn->toggleAction = [](const bool flag) { log("UNLOCK ALL!"); return false; };

    return btn;
}

Node* SettingsPage::getBlankButton() const
{
    const auto btn = Node::create();
    btn->setVisible(false);
    btn->setContentSize({0, 25 * config::getScaleFactor()});
    return btn;
}