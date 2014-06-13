#include "AboutPage.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include <boost/foreach.hpp>
#include <string>
#include <avalon/ui/parentalgate.h>
#include <avalon/utils/url.h>
#include <avalon/utils/platform.h>
#include "../buttons/ToggleButton.h"
#include "../utils/config.h"
#include "../utils/helper.h"
#include "../Alert.h"
#include "../utils/fonts.h"
#include "../utils/MyFlurry.h"
#include "../PageManager.h"

#if AVALON_PLATFORM_IS_TIZEN
	#include <FSystem.h>
#endif

using namespace cocos2d;

bool AboutPage::init()
{
    if (!Page::init()) {
        return false;
    }

    setBackground(Color3B::WHITE);
    initIcon();

    return true;
}

void AboutPage::addDescription()
{
    desc = fonts::createLight(_("settings", "about.desc").get(), 44, TextHAlignment::CENTER, TextVAlignment::CENTER, 66);
    addChild(desc);

    desc->setColor(Color3B::BLACK);
    desc->setOpacity(255);

    desc->setAnchorPoint({0.5, 0.5});
    desc->setPositionX(config::getFrameSize().width / 2);

    const auto totalHeight = config::getFrameSize().height;
    const auto iconHeight = icon->getPositionY() + (icon->getContentSize().height * icon->getScaleY() * 0.5);
    const auto heightLeft = totalHeight - iconHeight;
    desc->setPositionY(iconHeight + (heightLeft / 2.0));
}

void AboutPage::addButtons()
{
    const std::list<Node*> btns = {
        getFacebookButton(),
        getContactUsButton(),
#if !AVALON_PLATFORM_IS_TIZEN
        getDonateButton(),
#endif
        getOurAppsButton()
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

ToggleButton* AboutPage::getOurAppsButton() const
{
    const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return "about.ourapps"; };
    btn->detectState = []() { return true; };
    btn->toggleAction = [](const bool flag) {
        avalon::ui::parentalgate::showOnlyIos([]() {
#if AVALON_PLATFORM_IS_IOS
            avalon::utils::url::open("CHANGE_ME");
#elif AVALON_PLATFORM_IS_ANDROID_AMAZON
            avalon::utils::url::open("CHANGE_ME");
#elif AVALON_PLATFORM_IS_ANDROID_GOOGLE
            avalon::utils::url::open("CHANGE_ME");
#elif AVALON_PLATFORM_IS_ANDROID_SAMSUNG
            avalon::utils::url::open("CHANGE_ME");
#else
            avalon::utils::url::open("CHANGE_ME");
#endif
            MyFlurry::logEvent("moregames");
        });
        return false;
    };
    return btn;
}

ToggleButton* AboutPage::getContactUsButton() const
{
    const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return "about.contactus"; };
    btn->detectState = []() { return true; };
    btn->toggleAction = [](const bool flag) {
        avalon::ui::parentalgate::showOnlyIos([]() {
#if AVALON_PLATFORM_IS_TIZEN
            using Tizen::Base::Collection::HashMap;
            using Tizen::Base::String;
            using Tizen::App::AppControl;
            using Tizen::App::AppManager;

            HashMap extraData;
            extraData.Construct();
            String subjectKey = L"http://tizen.org/appcontrol/data/subject";
            String subjectVal = L"CHANGE_ME";
            String toKey = L"http://tizen.org/appcontrol/data/to";
            String toVal = L"CHANGE_ME";
            extraData.Add(&subjectKey, &subjectVal);
            extraData.Add(&toKey, &toVal);

            auto pAc = AppManager::FindAppControlN(L"tizen.email", L"http://tizen.org/appcontrol/operation/compose");
            if (pAc) {
                pAc->Start(null, null, &extraData, null);
                delete pAc;
            }
#else
            std::string mailto = "mailto:CHANGE_ME?subject=CHANGE_ME";
            avalon::utils::url::open(mailto.c_str());
#endif
            MyFlurry::logEvent("feedback");
        });
        return false;
    };
    return btn;
}

ToggleButton* AboutPage::getDonateButton()
{
    const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return "about.support"; };
    btn->detectState = []() { return true; };
    btn->toggleAction = [this](const bool flag) {
        avalon::ui::parentalgate::showOnlyIos([this]() {
            auto payment = avalon::payment::Loader::globalManager;
            if (helper::paymentAvailableCheck(payment.get())) {
                payment->delegate = this;
                payment->getProduct("support")->purchase();
            }
            MyFlurry::logEvent("support");
        });
        return false;
    };
    return btn;
}

ToggleButton* AboutPage::getFacebookButton() const
{
    const auto btn = ToggleButton::create();
    btn->getLabel = [](const bool flag) { return "about.facebook"; };
    btn->detectState = []() { return true; };
    btn->toggleAction = [](const bool flag) {
        avalon::ui::parentalgate::showOnlyIos([]() {
            avalon::utils::url::open("CHANGE_ME");
            MyFlurry::logEvent("facebook");
        });
        return false;
    };
    return btn;
}

void AboutPage::updateContainerLayout() const
{
    const auto spacing = 15 * config::getScaleFactorHeight();
    float nextPosY = 0;
    float maxWidth = 0;

    Object* it = nullptr;
    CCARRAY_FOREACH (container->getChildren(), it) {
        const auto btn = dynamic_cast<Node*>(it);
        if (!btn) {
            continue;
        }

        btn->setAnchorPoint(Point::ZERO);
        btn->setPositionY(nextPosY);

        nextPosY += btn->getContentSize().height + spacing;
        maxWidth = fmaxf(maxWidth, btn->getContentSize().width);
    }

    container->setContentSize({maxWidth, nextPosY - spacing});
    container->setAnchorPoint({0.5, 0.5});
    container->setPositionX(config::getFrameSize().width / 2);
    container->setPositionY(container->getContentSize().height * -0.5);
}

void AboutPage::initIcon()
{
    icon = Sprite::createWithSpriteFrameName("icon-coragames.png");
    addChild(icon);

    icon->setAnchorPoint({0.5, 0.5});
    icon->setPosition(Point(config::getFrameSize() / 2));
    icon->setScale(std::max(config::getScaleFactor(), config::getScaleFactorHeight()));

    // fix y position to mimic anchor point of 0
    icon->setPositionY(icon->getPositionY() + (icon->getContentSize().height * icon->getScaleY() * 0.5));
}

void AboutPage::toggleVisibility()
{
    touchable = false;
    if (!desc) {
        addDescription();
        addButtons();
    }

    const auto duration = 0.5;
    const auto containerPosX = container->getPositionX();
    const auto containerHeight = container->getContentSize().height;
    const auto containerUpperPosY = (icon->getPositionY() - (icon->getContentSize().height * icon->getScaleY() * 0.5f)) * 0.5f;
    const auto iconScale = icon->getScale();

    stopAllActions();
    icon->stopAllActions();
    desc->stopAllActions();
    container->stopAllActions();

    if (visible) {
        desc->runAction(EaseOut::create(FadeOut::create(duration), 3));
        container->runAction(MoveTo::create(duration, {containerPosX, containerHeight * -0.5f}));
    } else {
        desc->runAction(EaseIn::create(FadeIn::create(duration), 3));
        container->runAction(MoveTo::create(duration, {containerPosX, containerUpperPosY}));
    }
    icon->runAction(Sequence::create(
        ScaleTo::create(duration * 0.6 * 0.3, iconScale * 0.8),
        ScaleTo::create(duration * 0.6 * 0.7, iconScale),
        NULL
    ));
    runAction(Sequence::create(
        DelayTime::create(duration),
        CallFunc::create([this]() { touchable = true; }),
        NULL
    ));

    visible = !visible;
}

void AboutPage::onTouch(cocos2d::Touch& touch, cocos2d::Event& event)
{
    if (!touchable) {
        return;
    }

    const auto touchLocation = touch.getLocation();
    if (!icon->getBoundingBox().containsPoint(touchLocation)) {
        return;
    }

    toggleVisibility();
    SimpleAudioEngine::getInstance()->playEffect("click.mp3");
}

void AboutPage::onPurchaseSucceed(avalon::payment::Manager* const manager, avalon::payment::Product* const product)
{
    auto id = product->getProductId();

    if (id.find(".support") == std::string::npos) {
        log("unknown product id received: %s", id.c_str());
        return;
    }

    auto consumable = dynamic_cast<avalon::payment::ProductConsumable*>(product);
    if (!consumable) {
        return;
    }

    consumable->consume();
    purchased = true;

    MyFlurry::logEventWithType("purchase-succeed", "support");
}

void AboutPage::onPurchaseFail(avalon::payment::Manager* const manager)
{
    helper::showPaymentFailed();
    MyFlurry::logEventWithType("purchase-fail", "support");
}

void AboutPage::onTransactionStart(avalon::payment::Manager* const manager)
{
    helper::showPaymentPendingSpinner(true);
    MyFlurry::startTimedEvent("payment-transaction");
}

void AboutPage::onTransactionEnd(avalon::payment::Manager* const manager)
{
    auto payment = avalon::payment::Loader::globalManager;
    if (payment) {
        payment->delegate = nullptr;
    }
    
    helper::showPaymentPendingSpinner(false);
    MyFlurry::endTimedEvent("payment-transaction");

    if (!purchased) {
        return;
    }
    purchased = false;

    auto alert = Alert::create();
    Director::getInstance()->getRunningScene()->addChild(alert);
    alert->setDescription(_("settings", "about.support.thanks").get());
    alert->show([]() {}, true);
}
