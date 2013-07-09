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

    setBackground(ccWHITE);

    return true;
}

void GamePage::onTouch(cocos2d::Touch* touch, cocos2d::Event* event)
{
    manager->scrollUp();
}