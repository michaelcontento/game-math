#include "MainPage.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../PageManager.h"
#include "../Alert.h"

using namespace cocos2d;

bool MainPage::init()
{
    if (!Page::init()) {
        return false;
    }

    setBackground(Color3B::WHITE);
    addIcon();
    addTapToPlayLabel();

    return true;
}

void MainPage::addIcon()
{
    auto img = Sprite::createWithSpriteFrameName("icon-math.png");
    addChild(img);

    img->setAnchorPoint({0.5, 0});
    img->setPosition(Point(config::getFrameSize() / 2));

    img->setScale(std::max(config::getScaleFactor(), config::getScaleFactorHeight()));

    // fade
    img->setOpacity(0);
    img->runAction(EaseIn::create(FadeIn::create(0.75), 3));
}

void MainPage::addTapToPlayLabel()
{
    auto tapToPlay = fonts::createLight(_("general", "taptoplay").get(), 42, TextHAlignment::CENTER, TextVAlignment::CENTER, 66);
    addChild(tapToPlay);

    // color
    tapToPlay->setColor(Color3B::BLACK);

    // alignment
    tapToPlay->setAnchorPoint({0.5, 0.5});

    // positioning
    tapToPlay->setPositionX(config::getFrameSize().width / 2);
    tapToPlay->setPositionY(75 * config::getScaleFactor());

    // fade
    tapToPlay->setOpacity(0);
    tapToPlay->runAction(EaseIn::create(FadeIn::create(0.75), 3));
}

void MainPage::onTouch(cocos2d::Touch& touch, cocos2d::Event& event)
{
    PageManager::shared().scrollTo("category-1", config::getSnapAnimationDuration() * 2);
}
