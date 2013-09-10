#include "MoreGamesPage.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include <avalon/utils/url.h>
#include <avalon/utils/platform.h>
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

    upperIcon->setScale(config::getScaleFactor());
}

void MoreGamesPage::addLowerIcon()
{
    lowerIcon = Sprite::create("icon-down.png");
    addChild(lowerIcon);

    lowerIcon->setAnchorPoint({0.5, 0.5});
    lowerIcon->setPositionX(config::getFrameSize().width / 2);
    lowerIcon->setPositionY(config::getFrameSize().height / 4 * 1);

    lowerIcon->setScale(config::getScaleFactor());
}

void MoreGamesPage::addDescription()
{
    const auto label = fonts::createLight(_("general", "moregames").get(), 48);
    addChild(label);

    // color
    label->setColor(Color3B::BLACK);

    // alignment
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setVerticalAlignment(TextVAlignment::CENTER);

    // positioning
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
}