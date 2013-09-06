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
    if (!payment->getBackend().isPurchaseReady()) {
        auto alert = Alert::create();
        Director::getInstance()->getRunningScene()->addChild(alert);
        alert->setDescription(_("payment", "disabled").get());
        alert->show([]() {});
        return false;
    }

    if (!payment->getBackend().isInitialized()) {
        auto alert = Alert::create();
        Director::getInstance()->getRunningScene()->addChild(alert);
        alert->setDescription(_("payment", "notready").get());
        alert->show([]() {});
        return false;
    }

    return true;
}

void showPaymentPendingSpinner(const bool flag)
{
    if (lastPendingAlert) {
        lastPendingAlert->hide();
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

} // namespace helper
