#include "MoreGamesPage.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include <avalon/utils/url.h>
#include <avalon/utils/platform.h>
#include <avalon/ui/parentalgate.h>
#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../utils/MyFlurry.h"
#include "../PageManager.h"
#include "../Alert.h"

using namespace cocos2d;

bool MoreGamesPage::init()
{
    if (!Page::init()) {
        return false;
    }

    setBackground(Color3B::WHITE);
    addUpperIcon();
    addLowerIcon();
    addDescription();

    return true;
}

void MoreGamesPage::addUpperIcon()
{
    upperIcon = Sprite::create("icon-up.png");
    addChild(upperIcon);

    upperIcon->setAnchorPoint({0.5, 0.5});
    upperIcon->setPositionX(config::getFrameSize().width / 2);
    upperIcon->setPositionY(config::getFrameSize().height / 4 * 3);

    upperIcon->setScale(std::max(config::getScaleFactor(), config::getScaleFactorHeight()));
}

void MoreGamesPage::addLowerIcon()
{
    lowerIcon = Sprite::create("icon-down.png");
    addChild(lowerIcon);

    lowerIcon->setAnchorPoint({0.5, 0.5});
    lowerIcon->setPositionX(config::getFrameSize().width / 2);
    lowerIcon->setPositionY(config::getFrameSize().height / 4 * 1);

    lowerIcon->setScale(std::max(config::getScaleFactor(), config::getScaleFactorHeight()));
}

void MoreGamesPage::addDescription()
{
    const auto label = fonts::createLight(_("general", "moregames").get(), 42, TextHAlignment::CENTER, TextVAlignment::CENTER, 66);
    addChild(label);

    label->setColor(Color3B::BLACK);
    label->setAnchorPoint({0.5, 0.5});
    label->setPosition(Point(config::getFrameSize() / 2));
}

void MoreGamesPage::onTouch(cocos2d::Touch& touch, cocos2d::Event& event)
{
    const auto touchLocation = touch.getLocation();
    bool hit = false;
    
    if (upperIcon && upperIcon->getBoundingBox().containsPoint(touchLocation)) {
        hit = true;
    }
    if (lowerIcon && lowerIcon->getBoundingBox().containsPoint(touchLocation)) {
        hit = true;
    }

    if (!hit) {
        return;
    }

    avalon::ui::parentalgate::showOnlyIos([]() {
#if AVALON_PLATFORM_IS_IOS
        avalon::utils::url::open("http://target.georiot.com/Proxy.ashx?tsid=1302&GR_URL=https%3A%2F%2Fitunes.apple.com%2Fus%2Fartist%2Fcora-games%2Fid544717446");
#elif AVALON_PLATFORM_IS_ANDROID_AMAZON
        avalon::utils::url::open("amzn://apps/android?s=CoRa%20Games");
#elif AVALON_PLATFORM_IS_ANDROID_GOOGLE
        avalon::utils::url::open("market://search?q=pub:CoRa++Games");
#elif AVALON_PLATFORM_IS_ANDROID_SAMSUNG
        avalon::utils::url::open("samsungapps://SellerDetail/fnvddticys");
#endif
        MyFlurry::logEvent("moregames");
    });

    SimpleAudioEngine::getInstance()->playEffect("click.mp3");
}