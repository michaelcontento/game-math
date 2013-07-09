#include "CategoryPage.h"

#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../PageManager.h"
#include "GamePage.h"

using namespace cocos2d;

bool CategoryPage::init()
{
    if (!Page::init()) {
        return false;
    }

    setBackground({32, 202, 248});
    addHeadlineLabel();

    return true;
}

void CategoryPage::addHeadlineLabel()
{
    auto tapToPlay = fonts::createLight("ADDITION", 72);
    addChild(tapToPlay);

    // color
    tapToPlay->setColor(ccWHITE);

    // alignment
    tapToPlay->setAnchorPoint({0.5, 0.5});
    tapToPlay->setHorizontalAlignment(kTextAlignmentCenter);
    tapToPlay->setVerticalAlignment(kVerticalTextAlignmentCenter);

    // positioning
    tapToPlay->setPositionX(config::getFrameSize().width / 2);
    tapToPlay->setPositionY(config::getFrameSize().height - (50 * config::getScaleFactor()));
}

void CategoryPage::onTouch(cocos2d::Touch* touch, cocos2d::Event* event)
{
    manager->scrollDown(GamePage::create());
}