#include "MainPage.h"

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
    auto img = Sprite::create("icon-math.png");
    addChild(img);

    img->setAnchorPoint({0.5, 0});
    img->setPosition(config::getFrameSize() / 2);

    img->setScale(config::getScaleFactor());
}

void MainPage::addTapToPlayLabel()
{
    auto tapToPlay = fonts::createLight("tap to play", 42);
    addChild(tapToPlay);

    // color
    tapToPlay->setColor(Color3B::BLACK);

    // alignment
    tapToPlay->setAnchorPoint({0.5, 0.5});
    tapToPlay->setHorizontalAlignment(kTextAlignmentCenter);
    tapToPlay->setVerticalAlignment(kVerticalTextAlignmentCenter);

    // positioning
    tapToPlay->setPositionX(config::getFrameSize().width / 2);
    tapToPlay->setPositionY(75 * config::getScaleFactor());
}

void MainPage::onTouch(cocos2d::Touch& touch, cocos2d::Event& event)
{
    PageManager::shared().scrollTo("category-1", config::getSnapAnimationDuration() * 2);
}
