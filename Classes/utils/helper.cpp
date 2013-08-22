#include "helper.h"

#include "config.h"
#include "../Alert.h"
#include "../PageManager.h"

using namespace cocos2d;

namespace helper {

static Alert* lastPendingAlert = nullptr;

bool isNodeInViewRect(const cocos2d::Node& node)
{
    static const Rect viewRect {0, 0, config::getFrameSize().width, config::getFrameSize().height};
    return viewRect.containsPoint(node.convertToWorldSpace(node.getPosition()));
}

bool paymentAvailableCheck(avalon::payment::Manager* payment)
{
    if (!payment->getBackend().isInitialized()) {
        auto alert = Alert::create();
        Director::getInstance()->getRunningScene()->addChild(alert);
        alert->setDescription("payment not ready yet");
        alert->show([]() {});
        return false;
    }

    if (!payment->getBackend().isPurchaseReady()) {
        auto alert = Alert::create();
        Director::getInstance()->getRunningScene()->addChild(alert);
        alert->setDescription("payment disabled");
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

    lastPendingAlert->setDescription("shop request pending");
    lastPendingAlert->enableCloseOnTap(false);
    lastPendingAlert->show([]() {}, true);
}

} // namespace helper
