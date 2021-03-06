#include "SettingsPage.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include <boost/foreach.hpp>
#include <typeinfo>
#include <list>
#include <avalon/GameCenter.h>
#include <avalon/utils/platform.h>
#include <avalon/platform/android/gnustl_string_fixes.h>
#include <avalon/ui/parentalgate.h>
#include "../utils/config.h"
#include "../utils/user.h"
#include "../utils/helper.h"
#include "../utils/MyFlurry.h"
#include "../Alert.h"
#include "../buttons/ToggleButton.h"
#include "../PageManager.h"
#include "LockedCategoryPage.h"

using namespace cocos2d;
using namespace avalon;

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
#if !AVALON_PLATFORM_IS_TIZEN
        getRemoveAdsButton(),
        getUnlockAllButton(),
#endif
#if AVALON_PLATFORM_IS_IOS
        getRestoreButton(),
#endif
#if !AVALON_PLATFORM_IS_TIZEN
        getBlankButton(),
        getLeaderboardButton(),
        getAchievementsButton(),
#endif
        getBlankButton(),
#if AVALON_PLATFORM_IS_IOS || AVALON_PLATFORM_IS_TIZEN
        getVibrateButton(),
#endif
        getSoundButton(),
        getMusicButton()
    };

    container = Node::create();
    addChild(container);

    BOOST_FOREACH (auto& btn, btns) {
        if (btn != nullptr) {
            container->addChild(btn);
        }
    }

    updateContainerLayout();
}

void SettingsPage::showRemoveAds()
{
    auto buttonNode = container->getChildByTag(tagRemoveAdsButton);
    if (buttonNode) {
        return;
    }

    buttonNode = getRemoveAdsButton();
    if (!buttonNode) {
        return;
    }

    const auto spacing = 15 * config::getScaleFactorHeight();
    const auto offset = spacing + buttonNode->getContentSize().height;
    buttonNode->setPositionY(-offset);
    buttonNode->setAnchorPoint(Point::ZERO);
    container->addChild(buttonNode);

    auto size = container->getContentSize();
    size.height -= offset;
    container->setContentSize(size);
}

void SettingsPage::updateContainerLayout() const
{
#if AVALON_PLATFORM_IS_TIZEN
    const auto spacing = 25 * config::getScaleFactorHeight();
#else
    const auto spacing = 15 * config::getScaleFactorHeight();
#endif
    float nextPosY = 0;
    float maxWidth = 0;
    bool lastNodeWasToggleButton = false;
    std::list<Node*> removeBtns = {};

    Object* it = nullptr;
    CCARRAY_FOREACH (container->getChildren(), it) {
        const auto btn = dynamic_cast<Node*>(it);
        if (!btn) {
            continue;
        }

        const auto isToggleButton = (dynamic_cast<ToggleButton*>(it) != nullptr);
        if (!lastNodeWasToggleButton && !isToggleButton) {
            removeBtns.push_back(btn);
            continue;
        }
        lastNodeWasToggleButton = true;

        btn->setAnchorPoint(Point::ZERO);
        btn->setPositionY(nextPosY);

        nextPosY += btn->getContentSize().height + spacing;
        maxWidth = fmaxf(maxWidth, btn->getContentSize().width);
    }

    BOOST_FOREACH (auto& btn, removeBtns) {
        container->removeChild(btn);
    }

    container->setContentSize({maxWidth, nextPosY - spacing});
    container->setAnchorPoint({0.5, 0.5});
    container->setPosition(Point(config::getFrameSize() / 2));
}

ToggleButton* SettingsPage::getSoundButton() const
{
    const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return flag ? "sound.on" : "sound.off"; };
    btn->detectState = []() { return user::hasSoundEnabled(); };
    btn->toggleAction = [](const bool flag) { user::setSoundEnabled(flag); return true; };

    return btn;
}

ToggleButton* SettingsPage::getVibrateButton() const
{
	const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return flag ? "vibrate.on" : "vibrate.off"; };
    btn->detectState = []() { return user::useVibration(); };
    btn->toggleAction = [](const bool flag) { user::setUseVibration(flag); return true; };

    return btn;
}

ToggleButton* SettingsPage::getMusicButton() const
{
	const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return flag ? "music.on" : "music.off"; };
    btn->detectState = []() { return user::hasMusicEnabled(); };
    btn->toggleAction = [](const bool flag) { user::setMusicEnabled(flag); return true; };

    return btn;
}

ToggleButton* SettingsPage::getAchievementsButton() const
{
	const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return "achievements"; };
    btn->toggleAction = [](const bool flag) {
        if (!avalon::GameCenter().showAchievements()) {
            helper::showGameCenterAlert();
        }
        return false;
    };

    return btn;
}

ToggleButton* SettingsPage::getLeaderboardButton() const
{
	const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return "leaderboard"; };
    btn->toggleAction = [](const bool flag) {
        if (!avalon::GameCenter().showScores()) {
            helper::showGameCenterAlert();
        }
        return false;
    };

    return btn;
}

ToggleButton* SettingsPage::getRestoreButton()
{
    if (user::allLevelGroupsUnlocked() && !user::hasAdsEnabled()) {
        return nullptr;
    }

    const auto btn = ToggleButton::create();
    btn->setTag(tagRestorePurchases);
    btn->getLabel = [](const bool flag) { return "restore"; };
    btn->toggleAction = [this](const bool flag) {
        avalon::ui::parentalgate::showOnlyIos([this]() {
            auto payment = payment::Loader::globalManager;
            if (helper::paymentAvailableCheck(payment.get())) {
                helper::showPaymentPendingSpinner(true);
                payment->delegate = this;
                payment->restorePurchases();
            }
        });
        return false;
    };

    return btn;
}

ToggleButton* SettingsPage::getRemoveAdsButton()
{
    if (!user::hasAdsEnabled()) {
        return nullptr;
    }

    const auto btn = ToggleButton::create();
    btn->setTag(tagRemoveAdsButton);
    btn->getLabel = [](const bool flag) { return "removeads"; };
    btn->toggleAction = [this](const bool flag) {
        avalon::ui::parentalgate::showOnlyIos([this]() {
            auto payment = payment::Loader::globalManager;
            if (helper::paymentAvailableCheck(payment.get())) {
                helper::showPaymentPendingSpinner(true);
                payment->delegate = this;
                payment->getProduct("removeads")->purchase();
            }
        });
        return false;
    };

    return btn;
}

ToggleButton* SettingsPage::getUnlockAllButton()
{
    if (user::allLevelGroupsUnlocked()) {
        return nullptr;
    }

    const auto btn = ToggleButton::create();
    btn->setTag(tagUnlockAllButton);
    btn->getLabel = [](const bool flag) { return "unlockall"; };
    btn->toggleAction = [this](const bool flag) {
        if (user::allLevelGroupsUnlocked()) {
            container->removeChildByTag(tagUnlockAllButton);
            updateContainerLayout();
            return false;
        }

        int locked = 0;
        for (int i = 1; i <= 4; ++i) {
            if (user::isLevelGroupLocked(i)) {
                ++locked;
            }
        }
        auto key = std::string("all.") + std::to_string(locked);

        avalon::ui::parentalgate::showOnlyIos([this, key]() {
            auto payment = payment::Loader::globalManager;
            if (helper::paymentAvailableCheck(payment.get())) {
                helper::showPaymentPendingSpinner(true);
                payment->delegate = this;
                payment->getProduct(key.c_str())->purchase();
            }
        });
        return false;
    };

    return btn;
}

Node* SettingsPage::getBlankButton() const
{
    const auto btn = Node::create();
    btn->setVisible(false);
    btn->setContentSize({0, 25 * config::getScaleFactor()});
    return btn;
}

void SettingsPage::unlockPage(const int nbr, avalon::payment::Manager* const manager, avalon::payment::Product* const product)
{
    if (!user::isLevelGroupLocked(nbr)) {
        return;
    }

    auto key = std::string("category-") + std::to_string(nbr * 2 + 1);
    auto page = PageManager::shared().getPage(key.c_str());

    auto categoryPage = dynamic_cast<LockedCategoryPage*>(page);
    if (categoryPage) {
        categoryPage->unlock();
    }
}

void SettingsPage::onPurchaseSucceed(avalon::payment::Manager* const manager, avalon::payment::Product* const product)
{
    auto id = product->getProductId();

    if (id.find(".pack.") != std::string::npos) {
        auto nbr = std::stoi(id.substr(id.size() - 1));
        unlockPage(nbr, manager, product);
    }

    if (id.find(".all.") != std::string::npos) {
        MyFlurry::stopLogging = true;
        unlockPage(1, manager, product);
        unlockPage(2, manager, product);
        unlockPage(3, manager, product);
        unlockPage(4, manager, product);
        MyFlurry::stopLogging = false;
        MyFlurry::logEventWithType("purchase-succeed", "unlockall");
    }

    if (id.find(".removeads") != std::string::npos) {
        user::setAdsEnabled(false);
        MyFlurry::logEventWithType("purchase-succeed", "removeads");
    }

    if (id.find(".support") != std::string::npos) {
        auto consumable = dynamic_cast<avalon::payment::ProductConsumable*>(product);
        if (consumable) {
            consumable->consume();
        }
        MyFlurry::logEventWithType("purchase-succeed", "support");
    }

    // ----

    bool updateLayout = false;
    bool noAds = !user::hasAdsEnabled();
    bool allLevels = user::allLevelGroupsUnlocked();

    if (noAds) {
        container->removeChildByTag(tagRemoveAdsButton);
        updateLayout = true;
    }

    if (allLevels && noAds) {
        container->removeChildByTag(tagRestorePurchases);
        updateLayout = true;
    }

    if (allLevels) {
        container->removeChildByTag(tagUnlockAllButton);
        updateLayout = true;
    }

    if (updateLayout) {
        updateContainerLayout();
    }
}

void SettingsPage::onPurchaseFail(avalon::payment::Manager* const manager)
{
    helper::showPaymentPendingSpinner(false);
    helper::showPaymentFailed();
    MyFlurry::logEvent("purchase-fail");
}

void SettingsPage::onTransactionStart(avalon::payment::Manager* const manager)
{
    MyFlurry::startTimedEvent("payment-transaction");
}

void SettingsPage::onTransactionEnd(avalon::payment::Manager* const manager)
{
    helper::showPaymentPendingSpinner(false);
    MyFlurry::endTimedEvent("payment-transaction");

    auto payment = payment::Loader::globalManager;
    if (payment) {
        payment->delegate = nullptr;
    }
}

void SettingsPage::onRestoreSucceed(avalon::payment::Manager* const manager)
{
    MyFlurry::logEvent("restore-succeed");
}

void SettingsPage::onRestoreFail(avalon::payment::Manager* const manager)
{
    helper::showPaymentPendingSpinner(false);
    helper::showPaymentFailed();
    MyFlurry::logEvent("restore-fail");
}
