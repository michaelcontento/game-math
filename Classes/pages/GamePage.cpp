#include "GamePage.h"

#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../PageManager.h"

using namespace cocos2d;

bool GamePage::init()
{
    if (!Page::init()) {
        return false;
    }

    setBackground(ccORANGE);
    CCLog("GamePage %p INIT", this);

    return true;
}

void GamePage::onTouch(cocos2d::Touch* touch, cocos2d::Event* event)
{
    manager->scrollup();
}

GamePage::~GamePage()
{
    CCLog("GamePage %p DELETE", this);
}