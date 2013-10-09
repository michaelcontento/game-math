#include "helper.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include <avalon/utils/platform.h>
#include "config.h"
#include "../Alert.h"
#include "../PageManager.h"

using namespace cocos2d;

namespace helper {

static Alert* lastPendingAlert = nullptr;

bool paymentAvailableCheck(avalon::payment::Manager* payment)
{
    auto notReady = []() {
        auto alert = Alert::create();
        Director::getInstance()->getRunningScene()->addChild(alert);
        alert->setDescription(_("payment", "notready").get());
        alert->show([]() {});
        return false;
    };

    if (!payment) {
        return notReady();
    }

    if (!payment->getBackend().isPurchaseReady()) {
        auto alert = Alert::create();
        Director::getInstance()->getRunningScene()->addChild(alert);
        alert->setDescription(_("payment", "disabled").get());
        alert->show([]() {});
        return false;
    }

    if (!payment->getBackend().isInitialized()) {
        return notReady();
    }

    return true;
}

void showPaymentFailed()
{
#if AVALON_PLATFORM_IS_ANDROID_AMAZON
    // Please remove all interstitial dialog boxes from the in-app purchase
    // workflow. Only Amazon Apps In-App Purchase messaging should be included
    // in the purchase flow. Please refer to the In-App Purchasing API
    // documentation in the Amazon Mobile App Distribution Portal
    // at https://developer.amazon.com/apps.
    //
    // Steps to Reproduce:
    //
    // 1.Install and Launch the app
    // 2.Scroll the page and Tap on play, it leads to IAP page
    // 3.Close the IAP page, it shows the message “last payment request failed”
#else
    auto alert = Alert::create();
    Director::getInstance()->getRunningScene()->addChild(alert);
    alert->setDescription(_("payment", "failed").get());
    alert->show([]() {}, true);
#endif
}

void showPaymentPendingSpinner(const bool flag)
{
    if (lastPendingAlert) {
        lastPendingAlert->hide(true);
        lastPendingAlert = nullptr;
    }

    if (!flag) {
        return;
    }

    lastPendingAlert = Alert::create();
    Director::getInstance()->getRunningScene()->addChild(lastPendingAlert);

    lastPendingAlert->setTimeout(20, []() {
        helper::showPaymentPendingSpinner(false);

        auto newAlert = Alert::create();
        Director::getInstance()->getRunningScene()->addChild(newAlert);

        newAlert->setDescription(_("payment", "timeout").get());
        newAlert->show([]() {});
    });


    lastPendingAlert->setDescription(_("payment", "pending").get());
    lastPendingAlert->enableCloseOnTap(false);
    lastPendingAlert->show([]() {}, true);
}

void showGameCenterAlert()
{
    auto alert = Alert::create();
    Director::getInstance()->getRunningScene()->addChild(alert);
    static const auto key = "gamecenterdisabled" + avalon::utils::platform::getFlavor();
    alert->setDescription(_("settings", key.c_str()).get());
    alert->show([]() {});
}

} // namespace helper