#include "HintButton.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include <avalon/platform/android/gnustl_string_fixes.h>
#include <avalon/ui/parentalgate.h>
#include "../utils/fonts.h"
#include "../utils/user.h"
#include "../utils/config.h"
#include "../utils/helper.h"
#include "../utils/MyFlurry.h"
#include "../pages/GamePage.h"
#include "../Alert.h"

using namespace cocos2d;

HintButton* HintButton::create(GamePage& game)
{
    HintButton* pRet = new HintButton();
    if (pRet && pRet->init(game)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool HintButton::init(GamePage& game)
{
    if (!Layer::init()) {
        return false;
    }

    ignoreAnchorPointForPosition(false);
    this->game = &game;

    addLabel();
    addIcon();

    return true;
}

void HintButton::addLabel()
{
    label = fonts::createLight(std::to_string(user::getHintKeys()), 42 * config::getScaleFactorHeight());
    addChild(label);

    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Point::ZERO);

    setContentSize(label->getContentSize());
}

void HintButton::addIcon()
{
    key = Sprite::createWithSpriteFrameName("key.png");
    addChild(key);

    key->setColor(Color3B::BLACK);
    key->setScale(0.5 * config::getScaleFactor() * config::getScaleFactorHeight());

    auto size = key->getContentSize() * key->getScale();
    float padding = 5 * config::getScaleFactor();
    label->setPositionX(size.width + padding);

    auto cs = getContentSize();
    cs.width += size.width;
    cs.width += padding;
    setContentSize(cs);

    key->setAnchorPoint({0, 0.5});
    key->setPositionY(cs.height / 2.0);
}

void HintButton::onEnter()
{
    Layer::onEnter();

    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setSwallowsTouches(true);
    setTouchEnabledWithFixedPriority(-100);
}

void HintButton::onExit()
{
    setTouchEnabled(false);

    Layer::onExit();
}

bool HintButton::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    const auto bb = key->getBoundingBox();
    const auto mid = Point(bb.getMidX(), bb.getMidY());
    const auto loc = convertTouchToNodeSpace(touch);
    return (loc - mid).getLength() <= 120;
}

void HintButton::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (user::getHintKeys() > 0) {
        if (!game->isStarted() || game->isTimeover() || !game->canBeRevealed()) {
            return;
        }

        user::addHintKeys(-1);
        label->setString(std::to_string(user::getHintKeys()).c_str());
        game->revealHint();
    } else {
        avalon::ui::parentalgate::showOnlyIos([this]() {
            auto payment = avalon::payment::Loader::globalManager;
            if (helper::paymentAvailableCheck(payment.get())) {
                payment->delegate = this;
                payment->purchase("hints");
            }
        });
    }

    SimpleAudioEngine::getInstance()->playEffect("click.mp3");
}

void HintButton::onPurchaseSucceed(avalon::payment::Manager* const manager, avalon::payment::Product* const product)
{
    auto consumable = dynamic_cast<avalon::payment::ProductConsumable*>(product);

    if (!consumable) {
        log("unknown product id purchased: %s", product->getProductId().c_str());
        return;
    }

    user::addHintKeys(consumable->getQuantity());
    consumable->consume();

    label->setString(std::to_string(user::getHintKeys()).c_str());
    MyFlurry::logEventWithType("purchase-succeed", "hints");
}

void HintButton::onPurchaseFail(avalon::payment::Manager* const manager)
{
    MyFlurry::logEventWithType("purchase-fail", "hints");
}

void HintButton::onTransactionStart(avalon::payment::Manager* const manager)
{
    game->pause();
    helper::showPaymentPendingSpinner(true);
    MyFlurry::startTimedEvent("payment-transaction");
}

void HintButton::onTransactionEnd(avalon::payment::Manager* const manager)
{
    helper::showPaymentPendingSpinner(false);
    game->resume(1.0);
    MyFlurry::endTimedEvent("payment-transaction");
}

void HintButton::alert()
{
    if (user::useBigHintAlert()) {
        user::setUseBigHintAlert(false);
        game->pause();

        auto alert = Alert::create();
        Director::getInstance()->getRunningScene()->addChild(alert);
        alert->setDescription(_("game", "explainhints").get());
        alert->show([this]() { game->resume(); });
    }

    stopAllActions();
    runAction(
        Repeat::create(
            Sequence::create(
                ScaleTo::create(0.3, 1.25),
                ScaleTo::create(0.3, 1.0),
                NULL
            ),
        2)
    );
}