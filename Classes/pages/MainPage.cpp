#include "MainPage.h"

#include "../utils/config.h"
#include "../utils/fonts.h"

using namespace cocos2d;

bool MainPage::init()
{
    if (!Page::init()) {
        return false;
    }

    auto background = LayerColor::create({255, 255, 255, 255});
    addChild(background);

    addTapToPlayLabel();

    return true;
}

void MainPage::addTapToPlayLabel()
{
    auto tapToPlay = fonts::createLight("tap to play", 36);
    addChild(tapToPlay);

    // color
    tapToPlay->setColor(ccBLACK);

    // alignment
    tapToPlay->setAnchorPoint({0.5, 0.5});
    tapToPlay->setHorizontalAlignment(kTextAlignmentCenter);
    tapToPlay->setVerticalAlignment(kVerticalTextAlignmentCenter);

    // positioning
    tapToPlay->setPositionX(config::getFrameSize().width / 2);
    tapToPlay->setPositionY(50 * config::getScaleFactor());
}