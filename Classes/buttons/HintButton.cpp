#include "HintButton.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include "../utils/fonts.h"
#include "../utils/user.h"
#include "../utils/config.h"
#include "../utils/helper.h"
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
    if (!Node::init()) {
        return false;
    }

    this->game = &game;

    addLabel();
    addIcon();

    return true;
}

void HintButton::addLabel()
{
    label = fonts::createLight(std::to_string(user::getHintKeys()), 48);
    addChild(label);

    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Point::ZERO);

    setContentSize(label->getContentSize());
}

void HintButton::addIcon()
{
    auto key = Sprite::createWithSpriteFrameName("key.png");
    addChild(key);

    key->setColor(Color3B::BLACK);
    key->setScale(0.5 * config::getScaleFactor());

    auto size = key->getContentSize() * key->getScale();
    float padding = 5  * config::getScaleFactor();
    label->setPositionX(size.width + padding);

    auto cs = getContentSize();
    cs.width += size.width;
    cs.width += padding;
    setContentSize(cs);

    key->setAnchorPoint({0, 0.5});
    auto heightDiff = (cs.height - key->getContentSize().height);
    key->setPositionY(heightDiff / 2.f * config::getScaleFactor());
}

void HintButton::onEnter()
{
    Node::onEnter();

    Director::getInstance()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, 0, true);
}

void HintButton::onExit()
{
    Director::getInstance()
        ->getTouchDispatcher()
        ->removeDelegate(this);

    Node::onExit();
}

bool HintButton::ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return boundingBox().containsPoint(touch->getLocation());
}

void HintButton::ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (user::getHintKeys() > 0) {
        if (!game->isStarted() || game->isTimeover() || !game->canBeRevealed()) {
            return;
        }

        user::addHintKeys(-1);
        label->setString(std::to_string(user::getHintKeys()).c_str());
        game->revealHint();
    } else {
        auto payment = avalon::payment::Loader::globalManager;
        payment->delegate = this;
        if (helper::paymentAvailableCheck(payment.get())) {
            payment->purchase("hints");
        }
    }
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
}

void HintButton::onPurchaseFail(avalon::payment::Manager* const manager)
{
}

void HintButton::onTransactionStart(avalon::payment::Manager* const manager)
{
    game->pause();
    helper::showPaymentPendingSpinner(true);
}

void HintButton::onTransactionEnd(avalon::payment::Manager* const manager)
{
    helper::showPaymentPendingSpinner(false);
    game->resume(1.0);
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